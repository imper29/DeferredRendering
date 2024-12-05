#include "SysRenderingFog.h"
#include <iostream>

SysRenderingFog::SysRenderingFog(Scene& scene) : SceneSystem(scene) {
	m_BlitMesh = Mesh("Assets/meshes/blit_quad.obj");
	m_FogShader = std::make_shared<Shader>(Shader(std::initializer_list{
		ShaderSource::FromFile("Assets/shaders/effects/fog_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/effects/fog_vert.glsl", ShaderStage::Vert)
	}));
	m_FogColor = Color32(0.0f, 0.0f, 0.3f, 1.0f);
}

void SysRenderingFog::OnEvtReceived(SysRenderingPipeline::EvtRenderCompositePostEffects& evt) {
	if (m_FogShader) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		auto mode = m_FogShader->Use();
		glUniform4fv( m_FogShader->GetUniformLocation("fogColor"), 1, float4(m_FogColor));
		glBindTextureUnit(0, evt.framebuffer.GetTexture(SysRenderingPipeline::FramebufferDepth)->GetTexture());
		m_BlitMesh.Draw(mode);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

void SysRenderingFog::SetFogColor(const Color32& color) {
	m_FogColor = color;
}
Color32 SysRenderingFog::GetFogColor() const {
	return m_FogColor;
}
