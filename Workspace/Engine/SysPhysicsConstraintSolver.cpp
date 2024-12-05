#include "SysPhysicsConstraintSolver.h"

#include "CmpConstraintSpring.h"
#include "CmpMass.h"
#include "CmpVelocity.h"
#include "CmpVelocityAngular.h"
#include "CmpTransformLocalToWorld.h"

static void ApplyAccelerationAtPoint(const float3& centerOfMass, CmpVelocity* velocity, CmpVelocityAngular* velocityAngular, const float3& acceleration, const float3& accelerationPoint) {
	if (velocity) {
		velocity->value = velocity->value + acceleration;
	}
	if (velocityAngular) {
		velocityAngular->value = velocityAngular->value + (accelerationPoint - centerOfMass).cross(acceleration);
	}
}

SysPhysicsConstraintSolver::SysPhysicsConstraintSolver(Scene& scene) : SceneSystem(scene) {

}

void SysPhysicsConstraintSolver::OnEvtReceived(Scene::EvtUpdate& evt) {
	for (auto [entity, spring] : scene.entities.view<CmpConstraintSpring>().each()) {
		if (scene.entities.valid(spring.aEntity) && scene.entities.valid(spring.bEntity)) {
			//Get entity components.
			auto aMass = scene.entities.try_get<CmpMass>(spring.aEntity);
			auto bMass = scene.entities.try_get<CmpMass>(spring.bEntity);
			auto aVelocity = scene.entities.try_get<CmpVelocity>(spring.aEntity);
			auto bVelocity = scene.entities.try_get<CmpVelocity>(spring.bEntity);
			auto aVelocityAngular = scene.entities.try_get<CmpVelocityAngular>(spring.aEntity);
			auto bVelocityAngular = scene.entities.try_get<CmpVelocityAngular>(spring.bEntity);
			auto aLocalToWorld = scene.entities.try_get<CmpTransformLocalToWorld>(spring.aEntity);
			auto bLocalToWorld = scene.entities.try_get<CmpTransformLocalToWorld>(spring.bEntity);
			//Ensure required components exist.
			if (aLocalToWorld && bLocalToWorld) {
				//Calculate masses.
				float aMassInverse = aMass ? 1.0f / aMass->value : 0.0f;
				float bMassInverse = bMass ? 1.0f / bMass->value : 0.0f;
				//Calculate anchor points in world space.
				float3 aAnchor = aLocalToWorld->value * float4(spring.aAnchor, 1.0f);
				float3 bAnchor = bLocalToWorld->value * float4(spring.bAnchor, 1.0f);
				//Calculate spring force in world space.
				float3 anchorOffset = bAnchor - aAnchor;
				float anchorDistance = anchorOffset.length();
				float springStrength = (anchorDistance - spring.length) * spring.strength;
				float3 springForce = anchorOffset * (springStrength / anchorDistance * evt.timestep);
				//Apply spring acceleration.
				ApplyAccelerationAtPoint(aLocalToWorld->value.getTranslation(), aVelocity, aVelocityAngular, springForce * aMassInverse, aAnchor);
				ApplyAccelerationAtPoint(bLocalToWorld->value.getTranslation(), bVelocity, bVelocityAngular, springForce * -bMassInverse, bAnchor);
			}
		}
	}
}
