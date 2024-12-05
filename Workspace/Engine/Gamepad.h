#pragma once
#include "SDL_gamecontroller.h"

class Gamepad
{
public:
	bool IsUp(SDL_GameControllerButton btn) const {
		return m_Buttons[btn] == 0;
	}
	bool IsDown(SDL_GameControllerButton btn) const {
		return m_Buttons[btn] != 0;
	}
	bool IsPressed(SDL_GameControllerButton btn) const {
		return m_Buttons[btn] != 0
			&& m_ButtonsPrevious[btn] == 0;
	}
	bool IsReleased(SDL_GameControllerButton btn) const {
		return m_Buttons[btn] == 0
			&& m_ButtonsPrevious[btn] != 0;
	}
	
	float GetAxis(SDL_GameControllerAxis axis) const {
		return m_Axes[axis];
	}
	float GetAxisPrev(SDL_GameControllerAxis axis) const {
		return m_AxesPrevious[axis];
	}
	float GetAxisDelta(SDL_GameControllerAxis axis) const {
		return m_Axes[axis] - m_AxesPrevious[axis];
	}

	void Refresh() {
		//Refresh if controller exists.
		if (m_Controller)
		{
			//Refresh axes.
			for (int i = 0; i <= SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX; ++i) {
				m_AxesPrevious[i] = m_Axes[i];
				m_Axes[i] = SDL_GameControllerGetAxis(m_Controller, SDL_GameControllerAxis(i));
			}
			//Refresh buttons.
			for (int i = 0; i <= SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX; ++i) {
				m_ButtonsPrevious[i] = m_Buttons[i];
				m_Buttons[i] = SDL_GameControllerGetButton(m_Controller, SDL_GameControllerButton(i));
			}
		}
	}

private:
	SDL_GameController* m_Controller;
	float m_Axes[SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX + 1];
	float m_AxesPrevious[SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX + 1];
	Uint8 m_Buttons[SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX + 1];
	Uint8 m_ButtonsPrevious[SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX + 1];

	friend class InputManager;
};
