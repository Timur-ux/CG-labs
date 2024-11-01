-- vertex
#version 330 core

in vec3 vertexPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
  gl_Position = lightSpaceMatrix * model * vec4(vertexPosition, 1.0);
}

-- fragment

#version 330 core

void main() {
 gl_FragDepth = gl_FragCoord.z;
}
