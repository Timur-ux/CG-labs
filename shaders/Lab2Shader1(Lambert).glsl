-- vertex

#version 330 core

#define LIGHTS 1

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

uniform LightData lights[LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

in vec3 vertexPosition;
in vec2 textureCoord;
in vec3 normal;


out VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace[LIGHTS];
  vec2 texCoord;
  vec3 n[LIGHTS]; // Вектора нормали
  vec3 l[LIGHTS]; // Направления на источники света
} vsOut;

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);

  gl_Position = perspective*p;
  vsOut.texCoord = textureCoord;
  vsOut.fragPosWorldSpace = model*vec4(vertexPosition, 1.0);

  for(int i = 0; i < LIGHTS; ++i) {
    vsOut.fragPosLightSpace[i] = lights[i].lightSpaceMatrix*vsOut.fragPosWorldSpace;
    vsOut.n[i] = normalize(lights[i].normalMatrix*normal);
    vsOut.l[i] = normalize(lights[i].lightPosition - vec3(p));
  }
}

-- fragment

#version 330 core

#define LIGHTS 1

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

uniform LightData lights[LIGHTS];

uniform sampler2D main_texture0;

in VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace[LIGHTS];
  vec2 texCoord;
  vec3 n[LIGHTS]; // Вектора нормали
  vec3 l[LIGHTS]; // Направления на источники света
} fsIn;

out vec4 color;

float bias = -0.001;
float shadowDepth(vec4 fragPosLightSpace, sampler2D shadowMap) {
  // Перспективно делим
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 
  // Приводим координаты к интервалу [0, 1]
  projCoords = 0.5 * projCoords + 0.5;

  float minDepth = texture(shadowMap, projCoords.xy).r;

  return minDepth;
}
// 1 -- полная тень
// 0 -- отсутствие тени
float calculateShadow(vec4 fragPosLightSpace, sampler2D shadowMap) {
  // Перспективно делим
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 
  // Приводим координаты к интервалу [0, 1]
  projCoords = 0.5 * projCoords + 0.5;

  float minDepth = texture(shadowMap, projCoords.xy).r;
  float curDepth = fragPosLightSpace.z;

  float shadow = curDepth + bias <= minDepth ? 0.0 : 1.0;
  shadow = minDepth;

  return shadow;
}

void main() {
  vec4 texColor = texture(main_texture0, fsIn.texCoord);
  color = vec4(0);
  for(int i = 0; i < LIGHTS; ++i) {
    
    vec3 n2 = normalize(fsIn.n[i]); // Нормируем интерполируемую нормаль
    vec3 l2 = normalize(fsIn.l[i]); // И направление на источник света
    float diff = max(dot(n2,l2), 0.0); // диффузионная составляющая света
    float shadow = calculateShadow(fsIn.fragPosLightSpace[i], lights[i].shadowMap_texture1);
     // color += ((lights[i].kAmbient +  lights[i].kDiffuse*diff)*lights[i].color * texColor);
    float minDepth = texture(lights[i].shadowMap_texture1, fsIn.texCoord).r;//shadowDepth(fsIn.fragPosLightSpace[i], lights[i].shadowMap_texture1);
    color = vec4(minDepth);
    // color = vec4(vec3(shadow), 1.0);
  }
}
