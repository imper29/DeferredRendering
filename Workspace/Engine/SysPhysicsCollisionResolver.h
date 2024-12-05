#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "CollisionInfo.h"

class SysPhysicsCollisionResolver : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsCollisionResolver(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;
private:
	std::vector<CollisionInfo>& collisions;
};
