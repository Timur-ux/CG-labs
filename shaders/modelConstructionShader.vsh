#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec4 inColor;

layout (location = 3) in vec3 vertexPosition;

void main() {

  //gl_Position = perspective*view*model*vec4(vertexPosition, 1.0);
  gl_Position = view*model*vec4(vertexPosition, 1.0);
  inColor = gl_Position;
}
