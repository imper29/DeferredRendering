#pragma once
#include "entt/entt.hpp"
#include "float3.h"

struct CmpConstraintSpring {
	entt::entity aEntity;
	entt::entity bEntity;
	float3 aAnchor;
	float3 bAnchor;
	float strength;
	float length;

	CmpConstraintSpring(const entt::entity& aEntity, const entt::entity& bEntity, const float3& aAnchor, const float3& bAnchor, float strength, float length)
		: aEntity(aEntity), bEntity(bEntity), aAnchor(aAnchor), bAnchor(bAnchor), strength(strength), length(length) {

	}
};
