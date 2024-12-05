#pragma once
#include "entt/entt.hpp"
#include "vector"
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "float4x4.h"

class SceneSystem;
class Scene
{
public:
	Scene();
	virtual ~Scene();

	template<typename TSystem>
	bool HasSystem() {
		for (SceneSystem* system : systems) {
			TSystem* result = dynamic_cast<TSystem*>(system);
			if (result)
				return true;
		}
		return false;
	}
	template<typename TSystem>
	TSystem& GetSystem() {
		for (SceneSystem* system : systems) {
			TSystem* result = dynamic_cast<TSystem*>(system);
			if (result)
				return *result;
		}
		throw "System not found!";
	}
	template<typename TSystem, typename ...TArgs>
	TSystem& GetOrAddSystem(TArgs... args) {
		for (SceneSystem* system : systems) {
			TSystem* result = dynamic_cast<TSystem*>(system);
			if (result)
				return *result;
		}
		TSystem* system = new TSystem(*this, args...);
		systems.push_back(system);
		return *system;
	}

	template<typename TEvent>
	void SendMessage(TEvent& evt) {
		for (SceneSystem* system : systems) {
			SceneSystemListener<TEvent>* current = dynamic_cast<SceneSystemListener<TEvent>*>(system);
			if (current)
				current->OnEvtReceived(evt);
		}
	}
	template<typename TEvent>
	void SendMessage(const TEvent&& evt) {
		TEvent evtTemp = evt;
		SendMessage<TEvent>(evtTemp);
	}

	void Update(float timestep);
	void Render();

	entt::registry entities;
	std::vector<SceneSystem*> systems;
private:
	float m_Time;

public:
	/// <summary>
	/// Sent when updating the scene.
	/// </summary>
	class EvtUpdate {
	public:
		float time;
		float timestep;

		EvtUpdate(float time, float timestep)
			: time(time), timestep(timestep)
		{
		}
	};
	/// <summary>
	/// Sent before rendering the scene.
	/// </summary>
	class EvtRenderPre {
	public:
		float time;

		EvtRenderPre(float time)
			: time(time)
		{
		}
	};
	/// <summary>
	/// Sent when rendering the scene.
	/// </summary>
	class EvtRender {
	public:
		float time;

		EvtRender(float time)
			: time(time)
		{
		}
	};
	/// <summary>
	/// Sent after rendering the scene.
	/// </summary>
	class EvtRenderPost {
	public:
		float time;

		EvtRenderPost(float time)
			: time(time)
		{
		}
	};
};
