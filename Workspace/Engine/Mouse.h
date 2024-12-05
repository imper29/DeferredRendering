#pragma once
#include "SDL_mouse.h"

class Mouse
{
public:
	bool IsUp(Uint8 btn) const {
		return (m_Buttons & SDL_BUTTON(btn)) == 0;
	}
	bool IsDown(Uint8 btn) const {
		return (m_Buttons & SDL_BUTTON(btn)) != 0;
	}
	bool IsPressed(Uint8 btn) const {
		return (m_Buttons & SDL_BUTTON(btn)) != 0
			&& (m_ButtonsPrevious & SDL_BUTTON(btn)) == 0;
	}
	bool IsReleased(Uint8 btn) const {
		return (m_Buttons & SDL_BUTTON(btn)) == 0
			&& (m_ButtonsPrevious & SDL_BUTTON(btn)) != 0;
	}

	Uint8 GetButtons() const {
		return m_Buttons;
	}
	Uint8 GetButtonsPrev() const {
		return m_ButtonsPrevious;
	}

	int GetMousePosX() const {
		return m_PosX;
	}
	int GetMousePosY() const {
		return m_PosY;
	}
	int GetMousePosPrevX() const {
		return m_PosXPrev;
	}
	int GetMousePosPrevY() const {
		return m_PosYPrev;
	}
	int GetMousePosDeltaX() const {
		return m_PosX - m_PosXPrev;
	}
	int GetMousePosDeltaY() const {
		return m_PosY - m_PosYPrev;
	}

	void Refresh() {
		m_PosXPrev = m_PosX;
		m_PosYPrev = m_PosY;
		m_ButtonsPrevious = m_Buttons;
		m_Buttons = SDL_GetMouseState(&m_PosX, &m_PosY);
	}

private:
	int m_PosX, m_PosY;
	int m_PosXPrev, m_PosYPrev;
	Uint8 m_Buttons;
	Uint8 m_ButtonsPrevious;

	friend class InputManager;
};
