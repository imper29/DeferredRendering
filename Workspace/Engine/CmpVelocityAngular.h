#pragma once
#include "float3.h"

struct CmpVelocityAngular
{
	float3 value;

	CmpVelocityAngular() : value() {}
	CmpVelocityAngular(const float3& value) : value(value) {}
};
