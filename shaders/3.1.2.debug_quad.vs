#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = textureCoord;
    gl_Position = vec4(vertexPosition, 1.0);
}
