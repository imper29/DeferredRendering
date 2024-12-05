#pragma once
#include <memory>
#include "float4x4.h"
#include "Framebuffer.h"
#include "CmpTransformLocalToWorld.h"
#include "entt/entt.hpp"

struct CmpCamera {
	std::shared_ptr<Framebuffer> framebuffer;
	float clipPlaneNear, clipPlaneFar;
	float fieldOfView;
	bool orthographic;

	uint64_t layers;
	int64_t order;

	CmpCamera(float clipPlaneNear, float clipPlaneFar, float fieldOfView, bool orthographic = false, uint64_t layers = ~0U, int64_t order = 0, std::shared_ptr<Framebuffer> framebuffer = nullptr)
		: clipPlaneNear(clipPlaneNear), clipPlaneFar(clipPlaneFar)
		, fieldOfView(fieldOfView), orthographic(orthographic)
		, layers(layers), order(order), framebuffer(framebuffer) {}

	/// <summary>
	/// Returns the framebuffer for the given camera.
	/// </summary>
	/// <param name="registry">The entity registry.</param>
	/// <param name="entity">The camera entity.</param>
	/// <returns>The framebuffer for the given camera.</returns>
	static std::shared_ptr<Framebuffer> GetFramebuffer(entt::registry& registry, entt::entity entity) {
		CmpCamera* cam = registry.try_get<CmpCamera>(entity);
		if (cam) {
			return cam->framebuffer;
		}
		else {
			throw "The entity does not have a camera component.";
		}
	}
	/// <summary>
	/// Returns the projection matrix for the given camera.
	/// </summary>
	/// <param name="registry">The entity registry.</param>
	/// <param name="entity">The camera entity.</param>
	/// <returns>The projection matrix for the given camera.</returns>
	static float4x4 GetProjection(entt::registry& registry, entt::entity entity, float width, float height) {
		CmpCamera* cam = registry.try_get<CmpCamera>(entity);
		if (cam) {
			if (cam->orthographic)
				return float4x4::orthographic(width / height, cam->fieldOfView, cam->clipPlaneNear, cam->clipPlaneFar);
			else
				return float4x4::perspective(width / height, cam->fieldOfView, cam->clipPlaneNear, cam->clipPlaneFar);
		}
		else {
			throw "The entity does not have a camera component.";
		}
	}
	/// <summary>
	/// Returns the view matrix for the given camera.
	/// </summary>
	/// <param name="registry">The entity registry.</param>
	/// <param name="entity">The camera entity.</param>
	/// <returns>The view matrix for the given camera.</returns>
	static float4x4 GetView(entt::registry& registry, entt::entity entity) {
		CmpCamera* cam = registry.try_get<CmpCamera>(entity);
		if (cam) {
			CmpTransformLocalToWorld* ltw = registry.try_get<CmpTransformLocalToWorld>(entity);
			if (ltw) {
				return ltw->value.inverse();
			}
			else {
				throw "The entity does not have a transform local to world component.";
			}
		}
		else {
			throw "The entity does not have a camera component.";
		}
	}
	/// <summary>
	/// Returns the rendering layers for the given camera.
	/// </summary>
	/// <param name="registry">The entity registry.</param>
	/// <param name="entity">The camera entity.</param>
	/// <returns>The rendering layers for the given camera.</returns>
	static uint64_t GetLayers(entt::registry& registry, entt::entity entity) {
		CmpCamera* cam = registry.try_get<CmpCamera>(entity);
		if (cam) {
			return cam->layers;
		}
		else {
			throw "The entity does not have a camera component.";
		}
	}
};
