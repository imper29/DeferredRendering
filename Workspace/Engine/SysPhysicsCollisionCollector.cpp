#include "SysPhysicsCollisionCollector.h"

#include "CmpParent.h"
#include "CmpVelocity.h"
#include "CmpCollider.h"
#include "CmpTransformLocalToWorld.h"

static bool CheckTypes(ShapeType expected, ShapeType actualA, ShapeType actualB) {
	return expected == actualA && expected == actualB;
}
static bool CheckTypes(ShapeType expectedA, ShapeType expectedB, ShapeType actualA, ShapeType actualB, bool& aIsB) {
	if (expectedA == actualA && expectedB == actualB) {
		aIsB = false;
		return true;
	}
	else if (expectedA == actualB && expectedB == actualA) {
		aIsB = true;
		return true;
	}
	else {
		return false;
	}
}

static bool CalculateCollision(const ShapeSphere& aCollider, const ShapePlane& bCollider, CollisionInfo& output, entt::entity aEntity, entt::entity bEntity, const float3& aPosition, const float3& bPosition, const float3& aVelocity, const float3& bVelocity) {
	float3 rVel = aVelocity - bVelocity;
	float distance = bCollider.normal.dot(aPosition - bPosition);
	if (rVel.dot(bCollider.normal) < 0.0f && distance < aCollider.radius) {
		output = CollisionInfo(aEntity, bEntity, aPosition - bCollider.normal * distance, bCollider.normal, rVel, aCollider.radius - distance);
		return true;
	}
	return false;
}
static bool CalculateCollision(const ShapeSphere& aCollider, const ShapeSphere& bCollider, CollisionInfo& output, entt::entity aEntity, entt::entity bEntity, const float3& aPosition, const float3& bPosition, const float3& aVelocity, const float3& bVelocity) {
	auto rVel = aVelocity - bVelocity;
	auto rPos = aPosition - bPosition;
	auto dst = rPos.length();
	//They are moving towards eachother and are touching eachother.
	if (rVel.dot(rPos) < 0.0f && dst < aCollider.radius + bCollider.radius) {
		auto nrm = rPos / dst;
		output = CollisionInfo(aEntity, bEntity, aPosition + nrm * aCollider.radius, nrm, rVel, aCollider.radius + bCollider.radius - dst);
		return true;
	}
	return false;
}
static bool CalculateCollision(const ShapeSphere& aCollider, const ShapeAabb& bCollider, CollisionInfo& output, entt::entity aEntity, entt::entity bEntity, const float3& aPosition, const float3& bPosition, const float3& aVelocity, const float3& bVelocity) {
	auto rVel = aVelocity - bVelocity;
	auto rPos = aPosition - bPosition;
	auto point = math::clamp(aPosition, bPosition - bCollider.extents, bPosition + bCollider.extents);
	auto sphereToPoint = aPosition - point;
	auto sphereToPointLength = sphereToPoint.length();
	//They are moving towards eachother.
	if (rVel.dot(rPos) < 0.0f && sphereToPointLength < aCollider.radius) {
		auto nrm = sphereToPoint / sphereToPointLength;
		output = CollisionInfo(aEntity, bEntity, point, nrm, rVel, aCollider.radius - sphereToPointLength);
		return true;
	}
	return false;
}
//TODO: Implement collision between remaining shape pairs. I've only done plane-sphere, sphere-sphere, and aabb-sphere

SysPhysicsCollisionCollector::SysPhysicsCollisionCollector(Scene& scene) : SceneSystem(scene) {

}

void SysPhysicsCollisionCollector::OnEvtReceived(Scene::EvtUpdate& evt) {
	collisions.clear();
	{
		bool aIsB;
		CollisionInfo collision;
		auto group = scene.entities.group<>(entt::get<CmpTransformLocalToWorld, CmpVelocity, CmpCollider>, entt::exclude_t<CmpParent>());
		auto groupSize = group.size();
		for (auto a = group.begin(); a < group.end(); ++a) {
			auto [aLocalToWorld, aVelocity, aCollider] = group.get<>(*a);
			auto aPos = aLocalToWorld.value.getTranslation();
			for (auto b = a + 1; b < group.end(); ++b) {
				auto [bLocalToWorld, bVelocity, bCollider] = group.get<>(*b);
				if (CheckTypes(ShapeType::sphere, aCollider.type, bCollider.type)) {
					if (CalculateCollision(aCollider.sphere, aCollider.sphere, collision, *a, *b, aPos, bLocalToWorld.value.getTranslation(), aVelocity.value, bVelocity.value))
						collisions.push_back(collision);
				}
				else if (CheckTypes(ShapeType::sphere, ShapeType::aabb, aCollider.type, bCollider.type, aIsB)) {
					if (aIsB) {
						if (CalculateCollision(bCollider.sphere, aCollider.aabb, collision, *b, *a, bLocalToWorld.value.getTranslation(), aPos, bVelocity.value, aVelocity.value)) {
							collisions.push_back(collision);
						}
					}
					else {
						if (CalculateCollision(aCollider.sphere, bCollider.aabb, collision, *a, *b, aPos, bLocalToWorld.value.getTranslation(), aVelocity.value, bVelocity.value))
							collisions.push_back(collision);
					}
				}
				else if (CheckTypes(ShapeType::sphere, ShapeType::plane, aCollider.type, bCollider.type, aIsB)) {
					if (aIsB) {
						if (CalculateCollision(bCollider.sphere, aCollider.plane, collision, *b, *a, bLocalToWorld.value.getTranslation(), aPos, bVelocity.value, aVelocity.value)) {
							collisions.push_back(collision);
						}
					}
					else {
						if (CalculateCollision(aCollider.sphere, bCollider.plane, collision, *a, *b, aPos, bLocalToWorld.value.getTranslation(), aVelocity.value, bVelocity.value))
							collisions.push_back(collision);
					}
				}
			}
		}
	}
}
