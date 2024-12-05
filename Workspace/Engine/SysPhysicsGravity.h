#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"

class SysPhysicsGravity : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsGravity(Scene& scene);
	SysPhysicsGravity(Scene& scene, const float3& gravity);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;

	float3 gravity;
};
