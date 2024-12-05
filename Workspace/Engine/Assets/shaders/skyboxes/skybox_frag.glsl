#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inUvs;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, inUvs);
}
