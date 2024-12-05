#pragma once
#include "float3.h"

struct CmpScale
{
	float3 value;

	CmpScale() : value() {}
	CmpScale(const float3& value) : value(value) {}
};
