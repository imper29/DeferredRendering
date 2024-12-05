#pragma once
#include "float3.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
struct CollisionInfo {
	entt::entity aEntity;
	entt::entity bEntity;
	float3 contactPoint;
	float3 contactNormal;
	float3 contactVelocity;
	float contactDepth;
	bool resolved;

public:
	CollisionInfo() : aEntity(), bEntity(), contactPoint(), contactNormal(), contactVelocity(), contactDepth(), resolved(true) {}
	CollisionInfo(entt::entity aEntity, entt::entity bEntity, const float3& contactPoint, const float3& contactNormal, const float3& contactVelocity, float contactDepth) :
		aEntity(aEntity), bEntity(bEntity), contactPoint(contactPoint), contactNormal(contactNormal), contactVelocity(contactVelocity), contactDepth(contactDepth), resolved(false) {}

	void ResolveStandard(entt::registry& entityRegistry);
};
