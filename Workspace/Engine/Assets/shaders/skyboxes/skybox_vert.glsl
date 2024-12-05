#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inVertex;

layout(location = 0) uniform mat4 projectionViewModelMatrix;

layout(location = 0) out vec3 outUvs;

void main() {
    vec4 pos = projectionViewModelMatrix * vec4(inVertex, 1.0f);
    gl_Position = pos.xyww;
    outUvs = normalize(inVertex);
}
