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
  float kGlare; // Коэффициент блика 
  mat4 lightSpaceMatrix;
  sampler2D shadowMap_texture1;
};

uniform LightData lights[MAX_LIGHTS];


in VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace[MAX_LIGHTS];
  vec2 texCoord;
  vec3 n[MAX_LIGHTS]; // Вектора нормали
  vec3 l[MAX_LIGHTS]; // Направления на источники света
} fsIn;

float bias = 0.01;
// 1 -- полная тень
// 0 -- отсутствие тени
float calculateShadow(vec4 fragPosLightSpace, sampler2D shadowMap) {
  // Перспективно делим
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // Приводим координаты к интервалу [0, 1]
  projCoords = 0.5 * projCoords + 0.5;
  fragPosLightSpace = 0.5 * fragPosLightSpace + 0.5;

  // Расчет тени сделаем в виде свертки средним ядром
  float shadow = 0;
  float curDepth = fragPosLightSpace.z;
  vec2 texelSize = vec2(1) / textureSize(shadowMap, 0); 
  for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <=1; ++y) {
      float minDepth = texture(shadowMap, projCoords.xy + vec2(x, y)*texelSize).r;
      shadow += ((curDepth - bias <= minDepth) ? 0.0 : 1.0);
    }
  }
  shadow /= 9.0;

  // Для далеких теней не ставим тени
  if(projCoords.z >= 1)
    shadow = 0.0;

  return shadow;
}

// vec4 specColor = vec4(1, 0, 0, 1); // Для дебагга бликов
uniform vec3 cameraPos;
vec4 calculateLight(int i, vec4 texColor) {
    vec4 ambient = lights[i].color*lights[i].kAmbient;
    vec3 n2 = normalize(fsIn.n[i]); // Нормируем интерполируемую нормаль
    vec3 l2 = normalize(fsIn.l[i]); // И направление на источник света
    float diff = max(dot(n2,l2), 0.0); // диффузионная составляющая света
    vec4 diffuse = lights[i].kDiffuse*diff*lights[i].color;

    vec3 viewDir = normalize(cameraPos - fsIn.fragPosWorldSpace.xyz);

    vec3 halfwayDir = normalize(l2 + viewDir);
    float spec = pow(max(dot(n2, halfwayDir), 0.0), 64.0);
    vec4 specular = lights[i].kGlare*spec*lights[i].color;
    // vec4 specular = lights[i].kGlare*spec*specColor;

    float shadow = calculateShadow(fsIn.fragPosLightSpace[i], lights[i].shadowMap_texture1);

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor;
}

uniform sampler2D main_texture0;
out vec4 color;

void main() {
  vec4 texColor = texture(main_texture0, fsIn.texCoord);
  color = vec4(0);
  for(int i = 0; i < lightsCnt; ++i) {
     color += calculateLight(i, texColor);
  }
  // color /= float(lightsCnt);
}
