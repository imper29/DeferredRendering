#include "SysCameraFreemode.h"
#include "InputManager.h"

#include "CmpCameraFreecam.h"
#include "CmpPosition.h"
#include "CmpRotation.h"
#include "CmpTransformLocalToWorld.h"

static float2 euler = float2(0.0f);

SysCameraFreemode::SysCameraFreemode(Scene& scene) : SceneSystem(scene) {

}

void SysCameraFreemode::OnEvtReceived(SDL_Event& evt) {
	if (evt.type == SDL_EventType::SDL_MOUSEWHEEL) {
		for (auto [entity, camera] : scene.entities.view<CmpCameraFreecam>().each()) {
			camera.speedLinear += camera.speedLinear * evt.wheel.preciseY * 0.1f;
			camera.speedAngular += camera.speedAngular * evt.wheel.preciseX * 0.1f;
		}
	}
}
void SysCameraFreemode::OnEvtReceived(Scene::EvtUpdate& evt) {
	for (auto [entity, camera, position, rotation, localToWorld] : scene.entities.view<CmpCameraFreecam, CmpPosition, CmpRotation, CmpTransformLocalToWorld>().each()) {
		//Linear controls.
		auto& keyboard = InputManager::GetKeyboard(0);
		if ((keyboard.GetMods() & SDL_Keymod::KMOD_CTRL) == 0) {
			if (keyboard.IsDown(SDL_KeyCode::SDLK_d))
				position.value = position.value + localToWorld.value * float4(1, 0, 0, 0) * camera.speedLinear * evt.timestep;
			if (keyboard.IsDown(SDL_KeyCode::SDLK_a))
				position.value = position.value - localToWorld.value * float4(1, 0, 0, 0) * camera.speedLinear * evt.timestep;
			if (keyboard.IsDown(SDL_KeyCode::SDLK_s))
				position.value = position.value + localToWorld.value * float4(0, 0, 1, 0) * camera.speedLinear * evt.timestep;
			if (keyboard.IsDown(SDL_KeyCode::SDLK_w))
				position.value = position.value - localToWorld.value * float4(0, 0, 1, 0) * camera.speedLinear * evt.timestep;
			if (keyboard.IsDown(SDL_KeyCode::SDLK_LSHIFT))
				position.value = position.value - localToWorld.value * float4(0, 1, 0, 0) * camera.speedLinear * evt.timestep;
			if (keyboard.IsDown(SDL_KeyCode::SDLK_SPACE))
				position.value = position.value + localToWorld.value * float4(0, 1, 0, 0) * camera.speedLinear * evt.timestep;
		}
		//Angular controls.
		auto& mouse = InputManager::GetMouse(0);
		if (mouse.GetButtons() & SDL_BUTTON_RMASK) {
			euler.x -= mouse.GetMousePosDeltaY();
			euler.y -= mouse.GetMousePosDeltaX();
			quaternion rot = quaternion::fromEulerAngle(euler * camera.speedAngular * evt.timestep * math::DegToRad<float>);
			rotation.value = rot;
		}
	}
}
