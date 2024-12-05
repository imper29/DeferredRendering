#version 450

layout(vertices = 3) out;

layout(location = 0) in vec3 inNormal[];
layout(location = 1) in vec3 inTangent[];
layout(location = 2) in vec2 inUvs[];
layout(location = 3) in float inDistance[];

layout(location = 0) out vec3 outNormal[];
layout(location = 1) out vec3 outTangent[];
layout(location = 2) out vec2 outUvs[];
layout(location = 3) out vec3 outEyeWorldPosition[];

uniform mat4 model;
uniform mat4 view;

float inverseLerp(float v, float lo, float hi) {
	return (v - lo) / (hi - lo);
}
float lerp(float t, float lo, float hi) {
	return lo + (hi - lo) * t;
}
float tessLevel(vec3 eye, vec3 position) {
	float tessLerp = 1.0 - clamp(inverseLerp(length(eye - position), 5, 50), 0, 1);
	tessLerp = tessLerp * tessLerp;//2
	tessLerp = tessLerp * tessLerp;//4
	return lerp(tessLerp, 1, 16);
}

void main() {
	vec3 eye = (inverse(view) * vec4(0,0,0,1)).xyz;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	outNormal[gl_InvocationID] = inNormal[gl_InvocationID];
	outTangent[gl_InvocationID] = inTangent[gl_InvocationID];
	outUvs[gl_InvocationID] = inUvs[gl_InvocationID];
    outEyeWorldPosition[gl_InvocationID] = eye;

	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = tessLevel(eye, (p0 + p1 + p2) * 0.3333);
		gl_TessLevelOuter[0] = tessLevel(eye, (p1 + p2) * 0.5);//Ensures tess levels on triangle borders are aligned. Otherwise, holes would appear.
		gl_TessLevelOuter[1] = tessLevel(eye, (p0 + p2) * 0.5);//Ensures tess levels on triangle borders are aligned. Otherwise, holes would appear.
		gl_TessLevelOuter[2] = tessLevel(eye, (p0 + p1) * 0.5);//Ensures tess levels on triangle borders are aligned. Otherwise, holes would appear.
	}
}
