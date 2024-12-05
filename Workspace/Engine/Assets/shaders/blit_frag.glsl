#version 450

layout(location = 0) in vec2 inUVCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D source;

void main() {
    fragColor = texture(source, inUVCoord);
}
