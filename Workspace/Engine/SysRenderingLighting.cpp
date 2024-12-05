#include "SysRenderingLighting.h"
#include "CmpLightPoint.h"
#include "CmpLightAmbient.h"
#include "CmpLightDirectional.h"
#include "CmpTransformLocalToWorld.h"

SysRenderingLighting::SysRenderingLighting(Scene& scene) : SceneSystem(scene)
	, m_PointShader({
		ShaderSource::FromFile("Assets/shaders/lighting/point_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/lighting/point_vert.glsl", ShaderStage::Vert)})
	, m_DirectionalShader({
		ShaderSource::FromFile("Assets/shaders/lighting/directional_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/lighting/directional_vert.glsl", ShaderStage::Vert)})
	, m_AmbientShader({
		ShaderSource::FromFile("Assets/shaders/lighting/ambient_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/lighting/ambient_vert.glsl", ShaderStage::Vert)})
	, m_OutputShader({
		ShaderSource::FromFile("Assets/shaders/lighting/output_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/lighting/output_vert.glsl", ShaderStage::Vert)})
	, m_BlitMesh("Assets/meshes/blit_quad.obj") {

}

void SysRenderingLighting::OnEvtReceived(SysRenderingPipeline::EvtRenderComposite& evt) {
	//TODO: Collect light sources into an ssbo to reduce draw calls. Do this during renderpre if possible.
	//TOOD: Render small quads for each light source with instanced rendering instead of rendering a full-screen pass for each light source.

	//Texture binding.
	glBindTextureUnit(1, evt.framebuffer.GetTexture(FramebufferAttachment::Color1)->GetTexture());
	glBindTextureUnit(2, evt.framebuffer.GetTexture(FramebufferAttachment::Color2)->GetTexture());
	glBindTextureUnit(3, evt.framebuffer.GetTexture(FramebufferAttachment::Color3)->GetTexture());
	glBindTextureUnit(4, evt.framebuffer.GetTexture(FramebufferAttachment::Color4)->GetTexture());
	glBindTextureUnit(5, evt.framebuffer.GetTexture(FramebufferAttachment::Color5)->GetTexture());
	glBindTextureUnit(6, evt.framebuffer.GetTexture(FramebufferAttachment::Color6)->GetTexture());
	//Ambient lighting.
	{
		//Calculate ambient light color.
		float3 color = 0.0f;
		for (auto [entity, light] : scene.entities.view<CmpLightAmbient>().each()) {
			if (light.layers & evt.layers) {
				color = color + light.color;
			}
		}
		//Render ambient lighting.
		auto tess = m_AmbientShader.Use();
		glUniform3fv(m_AmbientShader.GetUniformLocation("lightColor"), 1, color);
		m_BlitMesh.Draw(tess);
	}
	//Directional lighting.
	{
		auto tess = m_DirectionalShader.Use();
		glUniformMatrix4fv(m_DirectionalShader.GetUniformLocation("view"), 1, false, evt.view);
		auto lightColorLoc = m_DirectionalShader.GetUniformLocation("lightColor");
		auto lightDirectionLoc = m_DirectionalShader.GetUniformLocation("lightDirection");
		for (auto [entity, light, low] : scene.entities.view<CmpLightDirectional, CmpTransformLocalToWorld>().each()) {
			if (light.layers & evt.layers) {
				float3 lightDirection = low.value * float4(0.0f, 0.0f, 1.0f, 0.0f);
				glUniform3fv(lightColorLoc, 1, light.color);
				glUniform3fv(lightDirectionLoc, 1, lightDirection);
				m_BlitMesh.Draw(tess);
			}
		}
	}
	//Point lighting.
	{
		auto tess = m_PointShader.Use();
		glUniformMatrix4fv(m_DirectionalShader.GetUniformLocation("view"), 1, false, evt.view);
		auto lightColorLocation = m_PointShader.GetUniformLocation("lightColor");
		auto lightPositionLocation = m_PointShader.GetUniformLocation("lightPosition");
		for (auto [entity, light, low] : scene.entities.view<CmpLightPoint, CmpTransformLocalToWorld>().each()) {
			if (light.layers & evt.layers) {
				glUniform4fv(lightColorLocation, 1, float4(light.color, light.falloff.x));
				glUniform4fv(lightPositionLocation, 1, float4(low.value.getTranslation(), light.falloff.y));
				m_BlitMesh.Draw(tess);
			}
		}
	}
}
