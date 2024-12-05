#pragma once
#include "float3.h"

struct CmpDamping
{
	float3 value;

	CmpDamping() : value() {}
	CmpDamping(const float3& value) : value(value) {}
};
