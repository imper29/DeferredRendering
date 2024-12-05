#version 450

layout(triangles, equal_spacing, ccw) in;

layout(location = 0) in vec3 inNormal[];
layout(location = 1) in vec3 inTangent[];
layout(location = 2) in vec2 inUvs[];
layout(location = 3) in vec3 inEyeWorldPosition[];

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outEyeWorldPosition;
layout(location = 2) out vec2 outUvs;
layout(location = 3) out mat3 outTbn;

uniform mat4 projection;
uniform mat4 view;
uniform float displacementScale;
uniform sampler2D displacementMap;

vec2 interpolateVec2(vec2 v0, vec2 v1, vec2 v2) {
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec3 interpolateVec3(vec3 v0, vec3 v1, vec3 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec4 interpolateVec4(vec4 v0, vec4 v1, vec4 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

void main() {
    outEyeWorldPosition = inEyeWorldPosition[0];
    outUvs = interpolateVec2(inUvs[0], inUvs[1], inUvs[2]);

    vec3 n = normalize(interpolateVec3(inNormal[0], inNormal[1], inNormal[2]));
    vec3 t = normalize(interpolateVec3(inTangent[0], inTangent[1], inTangent[2]));
    vec3 b = normalize(cross(t, n));

    outTbn = mat3(t, b, n);

    vec3 displacement = n * (texture(displacementMap, outUvs).r * displacementScale);
    vec4 worldPosition = interpolateVec4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
    worldPosition.xyz += displacement;

    outWorldPosition = worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
}
