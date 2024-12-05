#version 450

layout(location = 0) in vec2 inUVCoord;

layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform vec4 fogColor;
layout(binding = 0) uniform sampler2D zBufferMap;

void main() {
    float zBuffer = texture(zBufferMap, inUVCoord).r;
    float fogIntensity = zBuffer;
    fogIntensity *= fogIntensity;//2
    fogIntensity *= fogIntensity;//4
    fogIntensity *= fogIntensity;//8
    fogIntensity *= fogIntensity;//16
    fogIntensity *= fogIntensity;//32
    fragColor = vec4(fogColor.rgb, fogIntensity);
}
