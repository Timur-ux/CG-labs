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
uniform float fongP; // Параметр Фонга

in vec2 texCoord;
in vec3 n;
in vec3 l;

out vec4 color;

void main() {
  vec3 n2 = normalize(n); // Нормируем интерполируемую нормаль
  vec3 l2 = normalize(l); // И направление на источник света
  float diff = max(dot(n2,l2), 0.0);
  vec3 r = 2*n2*dot(n2, l2) - l2;
  float diff2 = pow(max(0, dot(r, l2)), fongP);

  vec4 Il = vec4(1, 1, 1, 0.1);
  float ka = 0.2;
  float kd = 0.6;
  float ks = 0.2;
  vec4 Cs = vec4(0, 0, 1, 1);

  color = texture(texture0, texCoord) * Il *(ka + kd*diff) + ks*Cs*Il*diff2;
}
