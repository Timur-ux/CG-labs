-- vertex

#version 330 core

#define MAX_LIGHTS 10
uniform int lightsCnt;

struct LightData {
  mat3 normalMatrix; // Матрица нормализации нормалей
  vec3 lightPosition; // Позиция света
  vec4 color; // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare; // Коэффициент блика (не используется в модели Ламберта)

  mat4 lightSpaceMatrix;
  sampler2D shadowMap_texture1;
};

uniform LightData lights[MAX_LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;


out VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace[MAX_LIGHTS];
  vec2 texCoord;
  vec3 n[MAX_LIGHTS]; // Вектора нормали
  vec3 l[MAX_LIGHTS]; // Направления на источники света
} vsOut;

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);

  gl_Position = perspective*p ;
  vsOut.texCoord = textureCoord;
  vsOut.fragPosWorldSpace = model*vec4(vertexPosition, 1.0);

  for(int i = 0; i < lightsCnt; ++i) {
    vsOut.fragPosLightSpace[i] = lights[i].lightSpaceMatrix*vsOut.fragPosWorldSpace ;
    vsOut.n[i] = normalize(lights[i].normalMatrix * normal);
    vsOut.l[i] = normalize(lights[i].lightPosition - vec3(p));
  }
}

-- fragment

#version 330 core

#define MAX_LIGHTS 10
uniform int lightsCnt;

struct LightData {
  mat3 normalMatrix; // Матрица нормализации нормалей
  vec3 lightPosition; // Позиция света
  vec4 color; // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare; // Коэффициент блика (не используется в модели Ламберта)
  mat4 lightSpaceMatrix;
  sampler2D shadowMap_texture1;
};

uniform LightData lights[MAX_LIGHTS];

uniform sampler2D main_texture0;

in VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace[MAX_LIGHTS];
  vec2 texCoord;
  vec3 n[MAX_LIGHTS]; // Вектора нормали
  vec3 l[MAX_LIGHTS]; // Направления на источники света
} fsIn;

out vec4 color;


float bias = 0.01;
// 1 -- полная тень
// 0 -- отсутствие тени
float calculateShadow(vec4 fragPosLightSpace, sampler2D shadowMap) {
  // Перспективно делим
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 
  // Приводим координаты к интервалу [0, 1]
  projCoords = 0.5 * projCoords + 0.5;

  float minDepth = texture(shadowMap, projCoords.xy).r;
  fragPosLightSpace = 0.5 * fragPosLightSpace + 0.5;
  float curDepth = fragPosLightSpace.z;

  float shadow = ((curDepth - bias <= minDepth) ? 0.0 : 1.0);
  if(projCoords.z >= 1)
    shadow = 0.0;
  // shadow = minDepth;

  return shadow;
}

vec4 calculateLight(int i, vec4 texColor) {
    vec3 n2 = normalize(fsIn.n[i]); // Нормируем интерполируемую нормаль
    vec3 l2 = normalize(fsIn.l[i]); // И направление на источник света
    float diff = max(dot(n2,l2), 0.0); // диффузионная составляющая света
    float shadow = calculateShadow(fsIn.fragPosLightSpace[i], lights[i].shadowMap_texture1);
    vec4 color = ((lights[i].kAmbient +  (1 - shadow)*lights[i].kDiffuse*diff)*lights[i].color * texColor);

    return color;
}

void main() {
  vec4 texColor = texture(main_texture0, fsIn.texCoord);
  color = vec4(0);
  for(int i = 0; i < lightsCnt; ++i) {
     color += calculateLight(i, texColor);
  }
  color /= float(lightsCnt);
}
