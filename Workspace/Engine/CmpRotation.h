#pragma once
#include "quaternion.h"

struct CmpRotation
{
	quaternion value;

	CmpRotation() : value(quaternion::identity()) {}
	CmpRotation(const quaternion& value) : value(value) {}
};
