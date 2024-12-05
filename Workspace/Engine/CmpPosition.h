#pragma once
#include "float3.h"

struct CmpPosition
{
	float3 value;

	CmpPosition() : value() {}
	CmpPosition(const float3& value) : value(value) {}
};
