#include "SceneManager.h"
#include "ScenePhysicsTests.h"
#include "SceneGraphicsTests.h"
#include "SDL_keycode.h"

static std::shared_ptr<Scene> s_CurrentScene;
static std::shared_ptr<Scene> s_FutureScene;
static SDL_Window* s_Window;
static float s_CurrentTime;
static bool s_ChangeScenes;

static std::shared_ptr<Scene> CreateDefaultScene()
{
	return std::make_shared<ScenePhysicsTests>();
}

float SceneManager::GetTime()
{
	return s_CurrentTime;
}
SDL_Window* SceneManager::GetWindow()
{
	return s_Window;
}
std::shared_ptr<Scene> SceneManager::GetCurrentScene() {
	return s_CurrentScene;
}
std::shared_ptr<Scene> SceneManager::LoadScene(const std::shared_ptr<Scene>&& scene) {
	s_FutureScene = scene;
	s_ChangeScenes = true;
	return scene;
}

void SceneManager::OnEvtInit(SDL_Window* window) {
	s_Window = window;
	s_CurrentScene = CreateDefaultScene();
}
void SceneManager::OnEvt(SDL_Event& evt) {
	if (evt.type == SDL_EventType::SDL_KEYDOWN) {
		if (evt.key.keysym.mod & SDL_Keymod::KMOD_CTRL) {
			switch (evt.key.keysym.sym) {
			case SDL_KeyCode::SDLK_F1:
				LoadScene(std::make_shared<SceneGraphicsTests>());
				break;
			case SDL_KeyCode::SDLK_F2:
				LoadScene(std::make_shared<ScenePhysicsTests>());
				break;
			case SDL_KeyCode::SDLK_F3:
				break;
			case SDL_KeyCode::SDLK_F4:
				break;
			case SDL_KeyCode::SDLK_F5:
				break;
			case SDL_KeyCode::SDLK_F6:
				break;
			case SDL_KeyCode::SDLK_F7:
				break;
			case SDL_KeyCode::SDLK_F8:
				break;
			case SDL_KeyCode::SDLK_F9:
				break;
			case SDL_KeyCode::SDLK_F10:
				break;
			case SDL_KeyCode::SDLK_F11:
				break;
			case SDL_KeyCode::SDLK_F12:
				break;
			}
		}
	}

	std::shared_ptr<Scene> scene = GetCurrentScene();
	if (scene) {
		scene->SendMessage(evt);
	}
}
void SceneManager::OnEvtQuit() {
	s_Window = nullptr;
	s_FutureScene = nullptr;
	s_CurrentScene = nullptr;
}
void SceneManager::OnEvtRender() {
	std::shared_ptr<Scene> scene = GetCurrentScene();
	if (scene) {
		scene->Render();
	}
}
bool SceneManager::OnEvtFlushSceneTransition() {
	if (s_ChangeScenes) {
		s_ChangeScenes = false;
		s_CurrentScene = s_FutureScene;
		s_FutureScene = nullptr;
		return true;
	}
	return false;
}
void SceneManager::OnEvtUpdate(float timestep) {
	std::shared_ptr<Scene> scene = GetCurrentScene();
	if (scene)
		scene->Update(timestep);
}
