-- vertex

#version 330 core

#define LIGHTS 2

struct LightData {
  mat3 normalMatrix; // Матрица нормализации нормалей
  vec3 lightPosition; // Позиция света
  vec4 color; // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare; // Коэффициент блика (не используется в модели Ламберта)
};

uniform LightData lights[LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord;
out vec3 n[LIGHTS]; // Вектора нормали
out vec3 l[LIGHTS]; // Направления на источники света

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);
  gl_Position = perspective*p;
  texCoord = textureCoord;
  for(int i = 0; i < LIGHTS; ++i) {
    n[i] = normalize(lights[i].normalMatrix*normal);
    l[i] = normalize(lights[i].lightPosition - vec3(p));
  }
}

-- fragment

#version 330 core

#define LIGHTS 2

struct LightData {
  mat3 normalMatrix; // Матрица нормализации нормалей
  vec3 lightPosition; // Позиция света
  vec4 color; // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare; // Коэффициент блика (не используется в модели Ламберта)
};

uniform LightData lights[LIGHTS];

uniform sampler2D texture0;

in vec2 texCoord;
in vec3 n[LIGHTS];
in vec3 l[LIGHTS];

out vec4 color;

void main() {
  vec4 texColor = texture(texture0, texCoord);
  color = vec4(0);
  for(int i = 0; i < LIGHTS; ++i) {
    vec3 n2 = normalize(n[i]); // Нормируем интерполируемую нормаль
    vec3 l2 = normalize(l[i]); // И направление на источник света
    float diff = max(dot(n2,l2), 0.0); // диффузионная составляющая света
    color += (lights[i].kAmbient + lights[i].kDiffuse*diff)*lights[i].color * texColor;
  }
}
