#version 450

layout(location = 0) in vec4 inVertex;
layout(location = 3) in vec2 inUvs;

layout(location = 0) out vec2 outUvs;

void main() {
    gl_Position = inVertex;
    outUvs = inUvs;
}
