#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;

out vec2 texCoord;

void main() {
  gl_Position = perspective*view*model*vec4(vertexPosition, 1.0);
  texCoord = textureCoord;
}
