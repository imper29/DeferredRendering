#version 450

layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inUvs;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outTangent;
layout(location = 2) out vec2 outUvs;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    outNormal = normalize(model * vec4(inNormal, 0.0)).xyz;
    outTangent = normalize(model * vec4(inTangent, 0.0)).xyz;
    outUvs = inUvs;
    gl_Position = model * inVertex;
}
