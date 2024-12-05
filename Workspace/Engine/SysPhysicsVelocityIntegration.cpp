#include "SysPhysicsVelocityIntegration.h"

#include "CmpPosition.h"
#include "CmpRotation.h"
#include "CmpVelocity.h"
#include "CmpVelocityAngular.h"

SysPhysicsVelocityIntegration::SysPhysicsVelocityIntegration(Scene& scene) : SceneSystem(scene) {

}

void SysPhysicsVelocityIntegration::OnEvtReceived(Scene::EvtUpdate& evt) {
	//Linear velocity.
	for (auto [entity, position, velocity] : scene.entities.view<CmpPosition, CmpVelocity>().each()) {
		position.value = position.value + velocity.value * evt.timestep;
	}
	//Angular velocity.
	for (auto [entity, rotation, velocity] : scene.entities.view<CmpRotation, CmpVelocityAngular>().each()) {
		float length = velocity.value.length();
		if (length >= 0.0001f)
			rotation.value = rotation.value * quaternion::fromAxisAngle(velocity.value / length, length * evt.timestep);
	}
}
