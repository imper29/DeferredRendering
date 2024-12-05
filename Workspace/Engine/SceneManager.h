#pragma once
#include <memory>
#include "Scene.h"
#include "SDL.h"

class SceneManager
{
public:
	static float GetTime();
	static SDL_Window* GetWindow();
	static std::shared_ptr<Scene> GetCurrentScene();
	static std::shared_ptr<Scene> LoadScene(const std::shared_ptr<Scene>&& scene);

	static void OnEvtInit(SDL_Window* window);
	static void OnEvt(SDL_Event& evt);
	static void OnEvtQuit();
	static void OnEvtRender();
	static bool OnEvtFlushSceneTransition();
	static void OnEvtUpdate(float timestep);
};
