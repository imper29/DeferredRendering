#pragma once
#include "float3.h"
struct ShapePlane {
	float3 normal;

	ShapePlane(const float3& normal) : normal(normal.normalized()) {}
};
