#pragma once
#include "SysRenderingPipeline.h"

class SysRenderingSkybox : public SceneSystem,
	public SceneSystemListener<SysRenderingPipeline::EvtRenderComposite> {

public:
	static constexpr size_t FramebufferOutputsCount = 1;
	static constexpr uint32_t FramebufferOutputs = {
		SysRenderingPipeline::FramebufferOutput
	};

	SysRenderingSkybox(Scene& scene);

	void OnEvtReceived(SysRenderingPipeline::EvtRenderComposite& evt);

	void SetSkyboxShader(const std::shared_ptr<Shader>& shader);
	std::shared_ptr<Shader> GetSkyboxShader();
	void SetSkyboxTexture(const std::shared_ptr<Texture>& texture);
	std::shared_ptr<Texture> GetSkyboxTexture();

private:
	Mesh m_BlitMesh;
	std::shared_ptr<Shader> m_SkyboxShader;
	std::shared_ptr<Texture> m_SkyboxTexture;
};
