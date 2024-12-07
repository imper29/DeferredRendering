#include "SysRenderingMeshRenderers.h"
#include "CmpTransformLocalToWorld.h"
#include "CmpMeshRenderer.h"

SysRenderingMeshRenderers::SysRenderingMeshRenderers(Scene& scene) : SceneSystem(scene) {

}

void SysRenderingMeshRenderers::OnEvtReceived(SysRenderingPipeline::EvtRenderPre& evt) {
	//TODO: Construct acceleration structure to support instanced rendering and frustum culling.
}
void SysRenderingMeshRenderers::OnEvtReceived(SysRenderingPipeline::EvtRenderShadows& evt) {
	//TODO: Render shadow casting geometry.
}
void SysRenderingMeshRenderers::OnEvtReceived(SysRenderingPipeline::EvtRenderGeometryOpaque& evt) {
	for (auto [entity, renderer, localToWorld] : scene.entities.view<CmpMeshRendererOpaque, CmpTransformLocalToWorld>().each()) {
		if (renderer.layers & evt.layers) {
			if (evt.frustum.contains(localToWorld.value.getTranslation(), renderer.mesh->GetRadius())) {
				auto tess = renderer.material->Use(evt.projection, evt.view, localToWorld.value);
				renderer.mesh->Draw(tess);
			}
		}
	}
}
void SysRenderingMeshRenderers::OnEvtReceived(SysRenderingPipeline::EvtRenderGeometryTransparent& evt) {
	//TODO: Add support for lighting to transparent geometry.
	for (auto [entity, renderer, localToWorld] : scene.entities.view<CmpMeshRendererTransparent, CmpTransformLocalToWorld>().each()) {
		if (renderer.layers & evt.layers) {
			if (evt.frustum.contains(localToWorld.value.getTranslation(), renderer.mesh->GetRadius())) {
				auto tess = renderer.material->Use(evt.projection, evt.view, localToWorld.value);
				renderer.mesh->Draw(tess);
			}
		}
	}
}
