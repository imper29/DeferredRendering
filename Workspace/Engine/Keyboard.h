#pragma once
#include "SDL_keyboard.h"

class Keyboard
{
public:
	bool IsUp(SDL_KeyCode key) const {
		return (m_KeyStates[SDL_GetScancodeFromKey(key)]) == 0;
	}
	bool IsDown(SDL_KeyCode key) const {
		return (m_KeyStates[SDL_GetScancodeFromKey(key)]) != 0;
	}
	bool IsPressed(SDL_KeyCode key) const {
		return (m_KeyStates[SDL_GetScancodeFromKey(key)]) != 0
			&& (m_KeyStatesPrevious[SDL_GetScancodeFromKey(key)]) == 0;
	}
	bool IsReleased(SDL_KeyCode key) const {
		return (m_KeyStates[SDL_GetScancodeFromKey(key)]) == 0
			&& (m_KeyStatesPrevious[SDL_GetScancodeFromKey(key)]) != 0;
	}

	SDL_Keymod GetMods() const {
		return m_Keymods;
	}
	SDL_Keymod GetModsPrev() const {
		return m_KeymodesPrevious;
	}

	void Refresh() {
		//Refresh key states.
		int keyNum;
		const Uint8* keyStates = SDL_GetKeyboardState(&keyNum);
		SDL_memcpy(m_KeyStatesPrevious, m_KeyStates, sizeof(Uint8) * keyNum);
		SDL_memcpy(m_KeyStates, keyStates, sizeof(Uint8) * keyNum);
		//Refresh key modifiers.
		m_KeymodesPrevious = m_Keymods;
		m_Keymods = SDL_GetModState();
	}

private:
	SDL_Keymod m_Keymods;
	SDL_Keymod m_KeymodesPrevious;
	Uint8 m_KeyStates[SDL_Scancode::SDL_NUM_SCANCODES];
	Uint8 m_KeyStatesPrevious[SDL_Scancode::SDL_NUM_SCANCODES];

	friend class InputManager;
};
