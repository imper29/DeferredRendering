#version 450

layout(location = 0) in vec4 inVertex;
layout(location = 3) in vec2 inUvs;

layout(location = 0) out vec2 outUvs;
layout(location = 1) out vec3 outEyeWorldPosition;

uniform mat4 view;

void main() {
    gl_Position = inVertex;
    outUvs = inUvs;
    outEyeWorldPosition = (inverse(view) * vec4(0,0,0,1)).xyz;
}
