#pragma once
#include "float4.h"

struct CmpLightPoint {
	float3 color;
	float2 falloff;
	uint64_t layers;

	CmpLightPoint(const float radius) : color(1.0f), falloff(0.0f, radius), layers(1) {

	}
	CmpLightPoint(const float3& color, const float2& falloff, const uint64_t& layers = 1) : color(color), falloff(falloff), layers(layers) {

	}
};
