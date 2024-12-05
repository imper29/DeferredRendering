#pragma once
#include "SceneSystem.h"
#include "SceneSystemListener.h"
#include "SysRenderingPipeline.h"

class SysRenderingMeshRenderers : public SceneSystem
	, public SceneSystemListener<SysRenderingPipeline::EvtRenderPre>
	, public SceneSystemListener<SysRenderingPipeline::EvtRenderShadows>
	, public SceneSystemListener<SysRenderingPipeline::EvtRenderGeometryOpaque>
	, public SceneSystemListener<SysRenderingPipeline::EvtRenderGeometryTransparent> {

public:
	SysRenderingMeshRenderers(Scene& scene);

	void OnEvtReceived(SysRenderingPipeline::EvtRenderPre& evt);
	void OnEvtReceived(SysRenderingPipeline::EvtRenderShadows& evt);
	void OnEvtReceived(SysRenderingPipeline::EvtRenderGeometryOpaque& evt);
	void OnEvtReceived(SysRenderingPipeline::EvtRenderGeometryTransparent& evt);
};
