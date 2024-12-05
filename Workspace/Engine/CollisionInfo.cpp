#include "CmpMass.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpVelocityAngular.h"
#include "CollisionInfo.h"
#include "CmpPhysicsMaterial.h"
#include <iostream>

//TODO: Stop using position component as world position. It is local position. That is okay right now because I don't process children in physics but I'd like to in the future.

//Note that this assumes a uniform mass distribution: Angular inertia is proportional to mass.
static void ApplyAccelerationAtPoint(const CmpPosition& position, CmpVelocity* velocity, CmpVelocityAngular* velocityAngular, const float3& deltaVelocity, const float3& accelerationPoint) {
	if (velocity) {
		velocity->value = velocity->value + deltaVelocity;
	}
	if (velocityAngular) {
		float3 offsetToCenterOfMass = accelerationPoint - position.value;
		float3 accelerationAngular = offsetToCenterOfMass.cross(deltaVelocity);
		velocityAngular->value = velocityAngular->value + accelerationAngular;
	}
}
static float GetInverseMass(entt::registry& r, entt::entity e) {
	CmpMass* m = r.try_get<CmpMass>(e);
	if (m == nullptr)
		return 0.0f; //mass is infinite so it cannot be moved.
	else
		return 1.0f / m->value;
}
static CmpPhysicsMaterial GetMaterial(entt::registry& r, entt::entity e) {
	CmpPhysicsMaterial* m = r.try_get<CmpPhysicsMaterial>(e);
	if (m == nullptr)
		return CmpPhysicsMaterial();
	return *m;
}

void CollisionInfo::ResolveStandard(entt::registry& entityRegistry) {
	//Math from: https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf
	resolved = true;
	if (entityRegistry.valid(aEntity) && entityRegistry.valid(bEntity)) {
		CmpVelocity* aVelocity = entityRegistry.try_get<CmpVelocity>(aEntity);
		CmpVelocity* bVelocity = entityRegistry.try_get<CmpVelocity>(bEntity);
		CmpPosition* aPosition = entityRegistry.try_get<CmpPosition>(aEntity);
		CmpPosition* bPosition = entityRegistry.try_get<CmpPosition>(bEntity);

		//DELTE ME LATER
		/*
		if (aPosition && aPosition->value.y < 0.8f)
			std::cout << aPosition->value.x << ", " << aPosition->value.y << ", " << aPosition->value.z << (int)aEntity << std::endl;
		if (bPosition && bPosition->value.y < 0.8f)
			std::cout << bPosition->value.x << ", " << bPosition->value.y << ", " << bPosition->value.z << " - " << (int)bEntity << std::endl;
		*/

		CmpVelocityAngular* aVelocityAngular = entityRegistry.try_get<CmpVelocityAngular>(aEntity);
		CmpVelocityAngular* bVelocityAngular = entityRegistry.try_get<CmpVelocityAngular>(bEntity);
		if (aPosition && bPosition) {
			//Generate physics material from pair of bodies.
			CmpPhysicsMaterial material = CmpPhysicsMaterial(GetMaterial(entityRegistry, aEntity), GetMaterial(entityRegistry, bEntity));
			//Determine masses of bodies.
			float aInverseMass = GetInverseMass(entityRegistry, aEntity);
			float bInverseMass = GetInverseMass(entityRegistry, bEntity);
			float sumInverseMass = aInverseMass + bInverseMass;



			//Determine collision acceleration for bodies.
			float3 aVelocityInitial = aVelocity ? aVelocity->value : 0.0f;
			float3 bVelocityInitial = bVelocity ? bVelocity->value : 0.0f;
			float collisionSpeed = contactNormal.dot(bVelocityInitial - aVelocityInitial);
			float collisionAcceleration = collisionSpeed / sumInverseMass * (1.0f + material.bounciness);
			//Apply collision acceleration for bodies.
			if (aVelocity)
				ApplyAccelerationAtPoint(*aPosition, aVelocity, aVelocityAngular, contactNormal * (collisionAcceleration * aInverseMass), contactPoint);
			if (bVelocity)
				ApplyAccelerationAtPoint(*bPosition, bVelocity, bVelocityAngular, contactNormal * (-collisionAcceleration * bInverseMass), contactPoint);
			
			
			
			//Determine friction acceleration for bodies.
			float3 aVelocityAtContact = aVelocityInitial + (aVelocityAngular ? aVelocityAngular->value : 0.0f).cross(contactPoint - aPosition->value);
			float3 bVelocityAtContact = bVelocityInitial + (bVelocityAngular ? bVelocityAngular->value : 0.0f).cross(contactPoint - bPosition->value);
			float3 frictionVector = bVelocityAtContact - aVelocityAtContact;
			frictionVector = (frictionVector - contactNormal * frictionVector.dot(contactNormal)) / sumInverseMass;
			//Apply friction acceleration for bodies.
			if (aVelocity)
				ApplyAccelerationAtPoint(*aPosition, aVelocity, aVelocityAngular, frictionVector * (material.friction * aInverseMass), contactPoint);
			if (bVelocity)
				ApplyAccelerationAtPoint(*bPosition, bVelocity, bVelocityAngular, frictionVector * (-material.friction * bInverseMass), contactPoint);
			
			
			
			//Apply offset for bodies to help cope with objects resting on eachother.
			//Otherwise, they can slowly slide into eachother when pressed together.
			if (aPosition != nullptr && bPosition != nullptr) {
				//THE PROBLEM IS HERE! NOW FIND IT!
				float depthOffset = contactDepth / sumInverseMass;
				aPosition->value = aPosition->value + contactNormal * (depthOffset * aInverseMass);
				bPosition->value = bPosition->value - contactNormal * (depthOffset * bInverseMass);
			}
		}
	}
}
