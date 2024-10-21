#version 330 core

uniform sampler2D textureData;

in vec2 texCoord;
out vec4 color;

void main() {
  color = texture(textureData, texCoord);
}
