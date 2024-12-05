#pragma once
#include "float2.h"
#include "Color32.h"
#include "SysRenderingPipeline.h"

class SysRenderingFog : public SceneSystem,
	public SceneSystemListener<SysRenderingPipeline::EvtRenderCompositePostEffects> {

public:
	static constexpr size_t FramebufferOutputsCount = 1;
	static constexpr uint32_t FramebufferOutputs = {
		SysRenderingPipeline::FramebufferOutput
	};

	SysRenderingFog(Scene& scene);

	void OnEvtReceived(SysRenderingPipeline::EvtRenderCompositePostEffects& evt);

	void SetFogColor(const Color32& color);
	Color32 GetFogColor() const;

private:
	Mesh m_BlitMesh;
	Color32 m_FogColor;
	float2 m_FogDistance;
	std::shared_ptr<Shader> m_FogShader;
};
