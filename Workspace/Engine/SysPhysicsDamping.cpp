#include "SysPhysicsDamping.h"
#include "CmpParent.h"

#include "CmpDamping.h"
#include "CmpVelocity.h"
#include "CmpDampingAngular.h"
#include "CmpVelocityAngular.h"

SysPhysicsDamping::SysPhysicsDamping(Scene& scene) : SceneSystem(scene) {

}

void SysPhysicsDamping::OnEvtReceived(Scene::EvtUpdate& evt) {
	//Linear Damping.
	for (auto [entity, velocity, damping] : scene.entities.view<CmpVelocity, CmpDamping>().each()) {
		velocity.value.x = velocity.value.x * powf(damping.value.x, evt.timestep);
		velocity.value.y = velocity.value.y * powf(damping.value.y, evt.timestep);
		velocity.value.z = velocity.value.z * powf(damping.value.z, evt.timestep);
	}
	//Angular Damping.
	for (auto [entity, velocity, damping] : scene.entities.view<CmpVelocityAngular, CmpDampingAngular>().each()) {
		velocity.value.x = velocity.value.x * powf(damping.value.x, evt.timestep);
		velocity.value.y = velocity.value.y * powf(damping.value.y, evt.timestep);
		velocity.value.z = velocity.value.z * powf(damping.value.z, evt.timestep);
	}
}
