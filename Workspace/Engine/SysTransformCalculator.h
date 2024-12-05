#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"

class SysTransformCalculator : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate> {
public:
	SysTransformCalculator(Scene& scene);

	void OnEvtReceived(Scene::EvtUpdate& evt) override;
};
