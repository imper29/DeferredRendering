#pragma once
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "SDL_events.h"

class SysCameraFreemode : public SceneSystem, public SceneSystemListener<Scene::EvtUpdate>, public SceneSystemListener<SDL_Event> {
public:
	SysCameraFreemode(Scene& scene);

	void OnEvtReceived(SDL_Event& evt) override;
	void OnEvtReceived(Scene::EvtUpdate& evt) override;
};
