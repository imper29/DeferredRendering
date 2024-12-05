#include "SysPhysicsGravity.h"
#include "CmpVelocity.h"
#include "CmpParent.h"

#include "TagGravity.h"
#include "CmpGravityScale.h"

SysPhysicsGravity::SysPhysicsGravity(Scene& scene) : SysPhysicsGravity(scene, float3(0.0f, -9.8f, 0.0f)) {

}

SysPhysicsGravity::SysPhysicsGravity(Scene& scene, const float3& gravity) : SceneSystem(scene), gravity(gravity) {

}

void SysPhysicsGravity::OnEvtReceived(Scene::EvtUpdate& evt) {
	float3 grav = gravity * evt.timestep;
	//Unscaled gravity.
	for (auto [entity, velocity] : 
		scene.entities.view<CmpVelocity, TagGravity>(entt::exclude_t<CmpParent, CmpGravityScale>()).each()) {
		velocity.value = velocity.value + grav;
	}
	//Scaled gravity.
	for (auto [entity, velocity, scale] : 
		scene.entities.view<CmpVelocity, CmpGravityScale, TagGravity>(entt::exclude_t<CmpParent>()).each()) {
		velocity.value = velocity.value + grav * scale.value;
	}
}
