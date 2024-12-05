#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "CollisionInfo.h"

class SysPhysicsCollisionCollector : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsCollisionCollector(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;

	std::vector<CollisionInfo> collisions;
};
