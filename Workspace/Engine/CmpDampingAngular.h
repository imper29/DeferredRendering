#pragma once
#include "float3.h"

struct CmpDampingAngular
{
	float3 value;

	CmpDampingAngular() : value() {}
	CmpDampingAngular(const float3& value) : value(value) {}
};
