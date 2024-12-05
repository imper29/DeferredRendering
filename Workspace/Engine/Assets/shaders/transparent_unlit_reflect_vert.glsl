#version 450

layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inUvs;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outEyeWorldPosition;
layout(location = 2) out vec2 outUvs;
layout(location = 3) out mat3 outTbn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 worldPosition = model * inVertex;
    outWorldPosition = worldPosition.xyz;
    outUvs = inUvs;

    vec3 bitangent = cross(inTangent, inNormal);
    vec3 t = normalize(vec3(model * vec4(inTangent, 0.0)));
    vec3 b = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 n = normalize(vec3(model * vec4(inNormal, 0.0)));
    outTbn = mat3(t, b, n);

    outEyeWorldPosition = (inverse(view) * vec4(0,0,0,1)).xyz;
    
    gl_Position = projection * view * worldPosition;
}
