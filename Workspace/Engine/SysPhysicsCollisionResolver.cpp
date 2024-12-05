#include "SysPhysicsCollisionResolver.h"
#include "SysPhysicsCollisionCollector.h"

SysPhysicsCollisionResolver::SysPhysicsCollisionResolver(Scene& scene) : SceneSystem(scene), collisions(scene.GetOrAddSystem<SysPhysicsCollisionCollector>().collisions) {

}

void SysPhysicsCollisionResolver::OnEvtReceived(Scene::EvtUpdate& evt) {
	for (auto& collision : collisions)
		if (!collision.resolved)
			collision.ResolveStandard(scene.entities);
}
