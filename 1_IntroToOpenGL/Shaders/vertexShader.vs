#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform vec3 displacement;

void main()
{
    vec3 finalPosition = position + displacement;    // WEIRD VERTEX STUFF
    vertexColor = color;

    gl_Position = vec4(finalPosition, 1.0);
}