#include "SysRenderingSkybox.h"
#include <iostream>

SysRenderingSkybox::SysRenderingSkybox(Scene& scene) : SceneSystem(scene)
, m_SkyboxShader(nullptr)
, m_SkyboxTexture(nullptr)
, m_BlitMesh("Assets/meshes/skybox_cube.obj") {

}

void SysRenderingSkybox::OnEvtReceived(SysRenderingPipeline::EvtRenderComposite& evt) {
	if (m_SkyboxShader && m_SkyboxTexture) {
		float4x4 projectionView = evt.projection * evt.view.withoutTranslation();

		glEnable(GL_DEPTH_TEST);
		auto tess = m_SkyboxShader->Use();
		glBindTextureUnit(0, m_SkyboxTexture->GetTexture());
		glUniformMatrix4fv(m_SkyboxShader->GetUniformLocation("projectionViewModelMatrix"), 1, false, projectionView);
		m_BlitMesh.Draw(tess);
		glDisable(GL_DEPTH_TEST);
	}
}

void SysRenderingSkybox::SetSkyboxShader(const std::shared_ptr<Shader>& shader) {
	m_SkyboxShader = shader;
}

std::shared_ptr<Shader> SysRenderingSkybox::GetSkyboxShader() {
	return m_SkyboxShader;
}

void SysRenderingSkybox::SetSkyboxTexture(const std::shared_ptr<Texture>& texture) {
	m_SkyboxTexture = texture;
}

std::shared_ptr<Texture> SysRenderingSkybox::GetSkyboxTexture() {
	return m_SkyboxTexture;
}
