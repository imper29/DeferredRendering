#version 450

layout(location = 0) in vec2 inUvs;

layout(location = 0) out vec4 fragColor;

layout(binding = 7) uniform sampler2D lightingMap;

void main() {
    fragColor = texture(lightingMap, inUvs);
}
