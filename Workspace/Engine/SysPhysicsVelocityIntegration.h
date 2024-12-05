#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"

class SysPhysicsVelocityIntegration : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsVelocityIntegration(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;
};
