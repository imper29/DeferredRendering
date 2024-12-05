#version 450

layout(location = 0) in vec4 inVertex;
layout(location = 3) in vec2 inUVCoord;

layout(location = 0) out vec2 outUVCoord;

void main() {
    gl_Position = inVertex;
    outUVCoord = inUVCoord;
}
