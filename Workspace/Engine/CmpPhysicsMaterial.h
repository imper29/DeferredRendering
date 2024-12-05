#pragma once
#include "math.h"
struct CmpPhysicsMaterial {
	float bounciness;
	float friction;

	CmpPhysicsMaterial() : bounciness(0.8f), friction(0.5f) {}
	CmpPhysicsMaterial(float bounciness, float friction) : bounciness(bounciness), friction(friction) {}
	CmpPhysicsMaterial(const CmpPhysicsMaterial& a, const CmpPhysicsMaterial& b) {
		bounciness = math::max<float>(a.bounciness, b.bounciness);
		friction = math::avg<float>(a.friction, b.friction);
	}
};
