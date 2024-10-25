#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
// uniform mat3 modelNorm; // Матрица преобразования направлений
// uniform vec3 lightPos; // Позиция источника света

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord;
//out vec3 n; // Вектор нормали
//out vec3 l; // Направление на источник света

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);
  gl_Position = perspective*p;
  // gl_Position = vec4(vertexPosition, 1.0);
  texCoord = textureCoord;
  //n = normalize(modelNorm*normal);
  //l = normalize(lightPos - p);
}
