#include "SysRenderingPipeline.h"
#include "SceneManager.h"
#include "CmpCamera.h"
#include <iostream>

static bool wireframe = false;
static size_t outputAttachment = 0;
constexpr static std::array<FramebufferAttachment, 8> outputAttachments = {
	FramebufferAttachment::Color0,
	FramebufferAttachment::Color1,
	FramebufferAttachment::Color2,
	FramebufferAttachment::Color3,
	FramebufferAttachment::Color4,
	FramebufferAttachment::Color5,
	FramebufferAttachment::Color6,
	FramebufferAttachment::Depth
};

SysRenderingPipeline::SysRenderingPipeline(Scene& scene) : SceneSystem(scene)
, m_BlitShader({
	ShaderSource::FromFile("Assets/shaders/blit_frag.glsl", ShaderStage::Frag),
	ShaderSource::FromFile("Assets/shaders/blit_vert.glsl", ShaderStage::Vert) })
, m_BlitMesh("Assets/meshes/blit_quad.obj") {
	glUniform1i(m_BlitShader.GetUniformLocation("source"), 0);
	SDL_Window* window = SceneManager::GetWindow();
	int width, height;
	SDL_GetWindowSizeInPixels(window, &width, &height);
	ResizeFramebuffer(width, height);
}

Framebuffer& SysRenderingPipeline::GetFramebuffer() {
	return *m_Framebuffer;
}

void SysRenderingPipeline::OnEvtReceived(SDL_Event& evt) {
	if (evt.type == SDL_EventType::SDL_WINDOWEVENT) {
		if (evt.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED) {
			SDL_Window* window = SDL_GetWindowFromID(evt.window.windowID);
			if (window == SceneManager::GetWindow()) {
				int x, y;
				SDL_GetWindowSizeInPixels(window, &x, &y);
				ResizeFramebuffer(x, y);
			}
		}
	}
	else if (evt.type == SDL_EventType::SDL_KEYDOWN) {
		if (evt.key.keysym.mod & SDL_Keymod::KMOD_CTRL) {
			if (evt.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT) {
				outputAttachment = (outputAttachment + outputAttachments.size() + 1) % outputAttachments.size();
				std::cout << "Rendering attachment #" << outputAttachment << std::endl;
			}
			else if (evt.key.keysym.sym == SDL_KeyCode::SDLK_LEFT) {
				outputAttachment = (outputAttachment + outputAttachments.size() - 1) % outputAttachments.size();
				std::cout << "Rendering attachment #" << outputAttachment << std::endl;
			}
			else if (evt.key.keysym.sym == SDL_KeyCode::SDLK_w) {
				wireframe = !wireframe;
			}
		}
	}
}
void SysRenderingPipeline::OnEvtReceived(Scene::EvtRender& evt) {
	//Window preparation.
	int width, height;
	SDL_Window* window = SceneManager::GetWindow();
	SDL_GetWindowSizeInPixels(window, &width, &height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	//Camera collection.
	std::vector<entt::entity> cameraEntities;
	for (auto entity : scene.entities.view<CmpCamera>()) {
		cameraEntities.push_back(entity);
	}
	//Camera sorting.
	std::sort(cameraEntities.begin(), cameraEntities.end(), [this](const entt::entity& a, const entt::entity& b) {
		const auto& aCam = this->scene.entities.get<CmpCamera>(a);
		const auto& bCam = this->scene.entities.get<CmpCamera>(b);
		return aCam.order < bCam.order;
		});
	//Camera rendering.
	for (auto cameraEntity : cameraEntities)
	{
		//Swap polygon mode.
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		//Calculate camera information.
		uint64_t layers = CmpCamera::GetLayers(scene.entities, cameraEntity);
		float4x4 view = CmpCamera::GetView(scene.entities, cameraEntity);
		float4x4 projection = CmpCamera::GetProjection(scene.entities, cameraEntity, (float)width, (float)height);
		std::shared_ptr<Framebuffer> framebuffer = CmpCamera::GetFramebuffer(scene.entities, cameraEntity);
		if (!framebuffer) {
			framebuffer = m_Framebuffer;
		}
		//Render pre.
		{
			//Bind and clear camera framebuffer.
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->GetFramebuffer());
			framebuffer->SetOutputs(EvtRenderPre::FramebufferOutputs);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(true);
			glEnable(GL_STENCIL_TEST);
			glStencilMask(~0U);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glClearColor(0, 0, 0, 1);
			//Render pre event.
			scene.SendMessage(EvtRenderPre(layers));
		}
		//Render shadows.
		{
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			framebuffer->SetOutputs(EvtRenderShadows::FramebufferOutputs);
			scene.SendMessage(EvtRenderShadows(layers));
		}
		//Render geometry opaque.
		{
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDepthMask(true);
			framebuffer->SetOutputs(EvtRenderGeometryOpaque::FramebufferOutputs);
			scene.SendMessage(EvtRenderGeometryOpaque(*framebuffer, projection, view, layers));
		}
		//Render composite pre effects.
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			framebuffer->SetOutputs(EvtRenderCompositePreEffects::FramebufferOutputs);
			scene.SendMessage(EvtRenderCompositePreEffects(*framebuffer, projection, view, layers));
		}
		//Render composite.
		{
			glBlendFunc(GL_ONE, GL_ONE);
			framebuffer->SetOutputs(EvtRenderComposite::FramebufferOutputs);
			scene.SendMessage(EvtRenderComposite(*framebuffer, projection, view, layers));
		}
		//Render geometry transparent.
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(false);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			framebuffer->SetOutputs(EvtRenderGeometryTransparent::FramebufferOutputs);
			scene.SendMessage(EvtRenderGeometryTransparent(*framebuffer, projection, view, layers));
		}
		//Render composite post effects.
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			framebuffer->SetOutputs(EvtRenderCompositePostEffects::FramebufferOutputs);
			scene.SendMessage(EvtRenderCompositePostEffects(*framebuffer, projection, view, layers));
		}
		//Render framebuffer output to actual output.
		if (framebuffer == m_Framebuffer) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			auto tess = m_BlitShader.Use();
			glBindTextureUnit(0, framebuffer->GetTexture(outputAttachments[outputAttachment])->GetTexture());
			if (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_BlitMesh.Draw(tess);
		}
	}
}

void SysRenderingPipeline::ResizeFramebuffer(int width, int height) {
	if (m_Framebuffer && width == m_Framebuffer->GetWidth() && height == m_Framebuffer->GetHeight())
		return;
	m_Framebuffer = std::make_shared<Framebuffer>(width, height);
	m_Output = std::make_shared<Texture2D>(width, height, GL_RGBA32F);
	m_Framebuffer->Attach(FramebufferOutput, m_Output);
	m_WorldPositions = std::make_shared<Texture2D>(width, height, GL_RGB32F);
	m_Framebuffer->Attach(FramebufferWorldPosition, m_WorldPositions);
	m_WorldNormals = std::make_shared<Texture2D>(width, height, GL_RGB32F);
	m_Framebuffer->Attach(FramebufferWorldNormal, m_WorldNormals);
	m_Albedo = std::make_shared<Texture2D>(width, height);
	m_Framebuffer->Attach(FramebufferAlbedo, m_Albedo);
	m_Emission = std::make_shared<Texture2D>(width, height);
	m_Framebuffer->Attach(FramebufferEmission, m_Emission);
	m_OcclusionRoughnessMetalness = std::make_shared<Texture2D>(width, height);
	m_Framebuffer->Attach(FramebufferOcclusionRoughnessMetalness, m_OcclusionRoughnessMetalness);
	m_Reflections = std::make_shared<Texture2D>(width, height);
	m_Framebuffer->Attach(FramebufferReflections, m_Reflections);
	m_StencilDepth = std::make_shared<TextureStencilDepth>(width, height);
	m_Framebuffer->Attach(FramebufferDepth, m_StencilDepth, 0);
}
