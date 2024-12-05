#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"

class SysPhysicsConstraintSolver : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysPhysicsConstraintSolver(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;
};
