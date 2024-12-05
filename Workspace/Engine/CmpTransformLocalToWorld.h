#pragma once
#include "float4x4.h"

struct CmpTransformLocalToWorld
{
	float4x4 value;

	CmpTransformLocalToWorld() : value(float4x4::identity()) {

	}
	CmpTransformLocalToWorld(const float4x4& value) : value(value) {

	}
};
