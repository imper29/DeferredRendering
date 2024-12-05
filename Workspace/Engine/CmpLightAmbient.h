#pragma once
#include "float3.h"

struct CmpLightAmbient {
	uint64_t layers;
	float3 color;

	CmpLightAmbient(const float3& color, const uint64_t& layers = 1) : color(color), layers(layers) {}
};
