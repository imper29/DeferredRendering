#pragma once
#include "SDL_events.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "TextureStencilDepth.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"

class SysRenderingPipeline : public SceneSystem,
	public SceneSystemListener<SDL_Event>,
	public SceneSystemListener<Scene::EvtRender> {
public:
	SysRenderingPipeline(Scene& scene);

	Framebuffer& GetFramebuffer();

	void OnEvtReceived(SDL_Event& evt) override;
	void OnEvtReceived(Scene::EvtRender& evt) override;

	static constexpr FramebufferAttachment FramebufferOutput = FramebufferAttachment::Color0;
	static constexpr FramebufferAttachment FramebufferWorldPosition = FramebufferAttachment::Color1;
	static constexpr FramebufferAttachment FramebufferWorldNormal = FramebufferAttachment::Color2;
	static constexpr FramebufferAttachment FramebufferAlbedo = FramebufferAttachment::Color3;
	static constexpr FramebufferAttachment FramebufferEmission = FramebufferAttachment::Color4;
	static constexpr FramebufferAttachment FramebufferOcclusionRoughnessMetalness = FramebufferAttachment::Color5;
	static constexpr FramebufferAttachment FramebufferReflections = FramebufferAttachment::Color6;
	static constexpr FramebufferAttachment FramebufferShadows = FramebufferAttachment::Color7;
	static constexpr FramebufferAttachment FramebufferDepth = FramebufferAttachment::Depth;

	/// <summary>
	/// Sent before the rendering pipeline executes for any given camera.
	/// </summary>
	struct EvtRenderPre {
		static constexpr std::array<FramebufferAttachment, 7> FramebufferOutputs = {
			FramebufferOutput,
			FramebufferWorldPosition,
			FramebufferWorldNormal,
			FramebufferAlbedo,
			FramebufferEmission,
			FramebufferOcclusionRoughnessMetalness,
			FramebufferReflections,
		};

		const uint64_t layers;

		EvtRenderPre(const uint64_t& layers) : layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering shadow maps.
	/// </summary>
	struct EvtRenderShadows {
		static constexpr std::array<FramebufferAttachment, 1> FramebufferOutputs = {
			FramebufferShadows
		};

		const uint64_t layers;

		EvtRenderShadows(const uint64_t& layers) : layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering opaque geometry to the gbuffer.
	/// </summary>
	struct EvtRenderGeometryOpaque {
		static constexpr std::array<FramebufferAttachment, 6> FramebufferOutputs = {
			FramebufferWorldPosition,
			FramebufferWorldNormal,
			FramebufferAlbedo,
			FramebufferEmission,
			FramebufferOcclusionRoughnessMetalness,
			FramebufferReflections
		};

		const Framebuffer& framebuffer;
		const float4x4& projection;
		const float4x4& view;
		const uint64_t layers;

		EvtRenderGeometryOpaque(const Framebuffer& framebuffer, const float4x4& projection, const float4x4& view, const uint64_t& layers) : framebuffer(framebuffer), projection(projection), view(view), layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering pre composite effects to the gbuffer. Effects like ambient occlusion.
	/// </summary>
	struct EvtRenderCompositePreEffects {
		static constexpr std::array<FramebufferAttachment, 1> FramebufferOutputs = {
			FramebufferOutput,
		};

		const Framebuffer& framebuffer;
		const float4x4& projection;
		const float4x4& view;
		const uint64_t layers;

		EvtRenderCompositePreEffects(const Framebuffer& framebuffer, const float4x4& projection, const float4x4& view, const uint64_t& layers) : framebuffer(framebuffer), projection(projection), view(view), layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering the composite to the gbuffer.
	/// </summary>
	struct EvtRenderComposite {
		static constexpr std::array<FramebufferAttachment, 1> FramebufferOutputs = {
			FramebufferOutput,
		};

		const Framebuffer& framebuffer;
		const float4x4& projection;
		const float4x4& view;
		const uint64_t layers;

		EvtRenderComposite(const Framebuffer& framebuffer, const float4x4& projection, const float4x4& view, const uint64_t& layers) : framebuffer(framebuffer), projection(projection), view(view), layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering transparent geometry to the gbuffer.
	/// Note that transparent geometry must use forward rendering.
	/// </summary>
	struct EvtRenderGeometryTransparent {
		static constexpr std::array<FramebufferAttachment, 1> FramebufferOutputs = {
			FramebufferOutput
		};

		const Framebuffer& framebuffer;
		const float4x4& projection;
		const float4x4& view;
		const uint64_t layers;

		EvtRenderGeometryTransparent(const Framebuffer& framebuffer, const float4x4& projection, const float4x4& view, const uint64_t& layers) : framebuffer(framebuffer), projection(projection), view(view), layers(layers) {

		}
	};
	/// <summary>
	/// Sent when rendering post composite effects to the gbuffer. Effects like bloom, vingette, color correcton, gamma correction.
	/// </summary>
	struct EvtRenderCompositePostEffects {
		static constexpr std::array<FramebufferAttachment, 1> FramebufferOutputs = {
			FramebufferOutput,
		};

		const Framebuffer& framebuffer;
		const float4x4& projection;
		const float4x4& view;
		const uint64_t layers;

		EvtRenderCompositePostEffects(const Framebuffer& framebuffer, const float4x4& projection, const float4x4& view, const uint64_t& layers) : framebuffer(framebuffer), projection(projection), view(view), layers(layers) {

		}
	};

private:
	void ResizeFramebuffer(int width, int height);

	Mesh m_BlitMesh;
	Shader m_BlitShader;
	std::shared_ptr<Texture2D> m_Output;
	std::shared_ptr<Texture2D> m_WorldPositions;
	std::shared_ptr<Texture2D> m_WorldNormals;
	std::shared_ptr<Texture2D> m_Albedo;
	std::shared_ptr<Texture2D> m_Emission;
	std::shared_ptr<Texture2D> m_OcclusionRoughnessMetalness;
	std::shared_ptr<Texture2D> m_Reflections;
	std::shared_ptr<TextureStencilDepth> m_StencilDepth;
	std::shared_ptr<Framebuffer> m_Framebuffer;
};
