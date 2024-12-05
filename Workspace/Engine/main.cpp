#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <GL/glew.h>
#include "SceneManager.h"
#include "InputManager.h"
#include <chrono>

SDL_Window* window;

SDL_GLContext context;
Sint32 eventCodeRender;
Sint32 eventCodeUpdate;
bool eventPendingRender;
bool eventPendingUpdate;
constexpr int millisecondsPerRender = 10;
constexpr int millisecondsPerUpdate = 10;
std::chrono::nanoseconds durationRender;
std::chrono::nanoseconds durationUpdate;

static Uint32 PostEventRender(Uint32 interval, void* param) {
	if (!eventPendingRender) {
		eventPendingRender = true;
		SDL_Event empty{};
		empty.type = SDL_EventType::SDL_USEREVENT;
		empty.user.code = eventCodeRender;
		SDL_PushEvent(&empty);
	}
	return interval;
}
static Uint32 PostEventUpdate(Uint32 interval, void* param) {
	if (!eventPendingUpdate) {
		eventPendingUpdate = true;
		SDL_Event empty{};
		empty.type = SDL_EventType::SDL_USEREVENT;
		empty.user.code = eventCodeUpdate;
		SDL_PushEvent(&empty);
	}
	return interval;
}

int main(int argc, char* argv[]) {
	//SDL initialization.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << SDL_GetError();
		return 1;
	}
	//Window initialization.
	window = SDL_CreateWindow("PhysicsAndGraphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
	//Opengl initialization.
	auto glewState = glewInit();
	if (glewState != GLEW_OK) {
		std::cout << glewGetErrorString(glewState);
		return 1;
	}
	//Manager initialization.
	InputManager::OnEvtInit();
	SceneManager::OnEvtInit(window);
	//Event loop initialization.
	eventCodeRender = SDL_RegisterEvents(1);
	eventCodeUpdate = SDL_RegisterEvents(1);
	SDL_AddTimer(millisecondsPerRender, PostEventRender, nullptr);
	SDL_AddTimer(millisecondsPerUpdate, PostEventUpdate, nullptr);
	//Event loop processing.
	SDL_Event evt;
	std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
	while (scene) {
		//Event handler.
		if (SDL_WaitEvent(&evt)) {
			if (evt.type == SDL_EventType::SDL_WINDOWEVENT && evt.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE) {
				SceneManager::LoadScene(nullptr);
			}
			else if (evt.type == SDL_EventType::SDL_CONTROLLERDEVICEADDED) {
				InputManager::OnEvtGameControllerAdded(evt.cdevice);
			}
			else if (evt.type == SDL_EventType::SDL_CONTROLLERDEVICEREMOVED) {
				InputManager::OnEvtGameControllerRemoved(evt.cdevice);
			}
			else if (evt.type == SDL_EventType::SDL_USEREVENT && evt.user.code == eventCodeRender) {
				auto timestampStart = std::chrono::high_resolution_clock::now();
				int x, y;
				SDL_GetWindowSize(window, &x, &y);
				glViewport(0, 0, x, y);
				SceneManager::OnEvtRender();
				SDL_GL_SwapWindow(window);
				eventPendingRender = false;
				auto timestampEnd = std::chrono::high_resolution_clock::now();
				durationRender = timestampEnd - timestampStart;
			}
			else if (evt.type == SDL_EventType::SDL_USEREVENT && evt.user.code == eventCodeUpdate) {
				auto timestampStart = std::chrono::high_resolution_clock::now();
				InputManager::Refresh();
				SceneManager::OnEvtUpdate(millisecondsPerUpdate / 1000.0f);
				eventPendingUpdate = false;
				auto timestampEnd = std::chrono::high_resolution_clock::now();
				durationUpdate = timestampEnd - timestampStart;
			}
			else {
				SceneManager::OnEvt(evt);
			}
		}
		//Flush scene transition then schedule immediate rendering if transition occurred.
		if (SceneManager::OnEvtFlushSceneTransition()) {
			scene = SceneManager::GetCurrentScene();
			PostEventRender(0, nullptr);
		}
		std::cout << "            \rmspf:" << std::chrono::duration_cast<std::chrono::milliseconds>(durationRender).count() << " mspu:" << std::chrono::duration_cast<std::chrono::milliseconds>(durationUpdate).count();
	}
	//Manager termination.
	SceneManager::OnEvtQuit();
	InputManager::OnEvtQuit();
	//Window termination.
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
