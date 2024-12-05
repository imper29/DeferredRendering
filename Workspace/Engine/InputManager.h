#pragma once
#include "Keyboard.h"
#include "Gamepad.h"
#include "Mouse.h"
#include "SDL_events.h"

class InputManager
{
public:
	static int GetKeyboardCount() {
		return s_KeyboardCount;
	}
	static Keyboard& GetKeyboard(int index) {
		return s_Keyboards[index];
	}
	static int GetGamepadCount() {
		return s_GamepadCount;
	}
	static Gamepad& GetGamepad(int index) {
		return s_Gamepads[index];
	}
	static int GetMouseCount() {
		return s_MouseCount;
	}
	static Mouse& GetMouse(int index) {
		return s_Mouses[index];
	}

	static void OnEvtInit();
	static void OnEvtQuit();
	static void OnEvtGameControllerRemoved(SDL_ControllerDeviceEvent evt);
	static void OnEvtGameControllerAdded(SDL_ControllerDeviceEvent evt);
	static void Refresh();

private:
	static constexpr int s_KeyboardCount = 1;
	static Keyboard s_Keyboards[s_KeyboardCount];

	static constexpr int s_GamepadCount = 4;
	static Gamepad s_Gamepads[s_GamepadCount];
	
	static constexpr int s_MouseCount = 1;
	static Mouse s_Mouses[s_MouseCount];
};
