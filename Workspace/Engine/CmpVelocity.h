#pragma once
#include "float3.h"

struct CmpVelocity
{
	float3 value;

	CmpVelocity() : value() {}
	CmpVelocity(const float3& value) : value(value) {}
};
