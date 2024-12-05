#include "InputManager.h"

Keyboard InputManager::s_Keyboards[s_KeyboardCount];
Gamepad InputManager::s_Gamepads[s_GamepadCount];
Mouse InputManager::s_Mouses[s_MouseCount];

void InputManager::OnEvtInit() {
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			s_Gamepads[i].m_Controller = SDL_GameControllerOpen(i);
		}
	}
}
void InputManager::OnEvtQuit() {
	for (int i = 0; i < s_GamepadCount; ++i) {
		if (s_Gamepads[i].m_Controller) {
			SDL_GameControllerClose(s_Gamepads[i].m_Controller);
			s_Gamepads[i].m_Controller = nullptr;
		}
	}
}
void InputManager::OnEvtGameControllerRemoved(SDL_ControllerDeviceEvent evt) {
	SDL_GameControllerClose(s_Gamepads[evt.which].m_Controller);
	s_Gamepads[evt.which].m_Controller = nullptr;
}
void InputManager::OnEvtGameControllerAdded(SDL_ControllerDeviceEvent evt) {
	s_Gamepads[evt.which].m_Controller = SDL_GameControllerOpen(evt.which);
}
void InputManager::Refresh() {
	for (int i = 0; i < s_KeyboardCount; ++i)
		s_Keyboards[i].Refresh();
	for (int i = 0; i < s_GamepadCount; ++i)
		s_Gamepads[i].Refresh();
	for (int i = 0; i < s_MouseCount; ++i)
		s_Mouses[i].Refresh();
}
