#version 330 core

uniform sampler2D texture0;

in vec2 texCoord;
out vec4 color;

void main() {
  color = texture(texture0, texCoord);
}
