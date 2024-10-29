-- vertex

#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat3 normalMatrix; // Матрица преобразования направлений
uniform vec3 lightPos; // Позиция источника света

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord;
out vec3 n; // Вектор нормали
out vec3 l; // Направление на источник света

float random2d(vec2 coord){
  return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);
  gl_Position = perspective*p;
  texCoord = textureCoord;
  n = normalize(normalMatrix*normal);
  l = normalize(lightPos - vec3(p));
}

-- fragment

#version 330 core

uniform sampler2D texture0;

in vec2 texCoord;
in vec3 n;
in vec3 l;

out vec4 color;

void main() {
  vec3 n2 = normalize(n); // Нормируем интерполируемую нормаль
  vec3 l2 = normalize(l); // И направление на источник света
  float diff = max(dot(n2,l2), 0.0);
  vec4 clr = vec4(1, 1, 1, 0.1);
  float ka = 0.2;
  float kd = 0.8;

  color = texture(texture0, texCoord) * ((ka + kd*diff) * clr);
}
