#pragma once
#include "SysRenderingPipeline.h"

class SysRenderingLighting : public SceneSystem
	, public SceneSystemListener<SysRenderingPipeline::EvtRenderComposite> {

public:
	SysRenderingLighting(Scene& scene);

	void OnEvtReceived(SysRenderingPipeline::EvtRenderComposite& evt);

private:
	Mesh m_BlitMesh;
	Shader m_OutputShader;
	Shader m_AmbientShader;
	Shader m_DirectionalShader;
	Shader m_PointShader;
};
