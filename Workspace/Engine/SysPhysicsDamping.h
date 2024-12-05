#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"

class SysPhysicsDamping : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsDamping(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;
};
