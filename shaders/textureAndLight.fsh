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
  vec4 clr = vec4(0.7, 0.1, 0.1, 0.1);
  float ka = 0.2;
  float kd = 0.8;

  color = mix(texture(texture0, texCoord), (ka + kd*diff) * clr);
}
