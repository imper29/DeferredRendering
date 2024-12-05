#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"

struct CmpMeshRendererShadow {
	std::shared_ptr<Mesh> mesh;
	uint64_t layers;

	CmpMeshRendererShadow(const std::shared_ptr<Mesh>& mesh, const uint64_t& layers = 1) : mesh(mesh), layers(layers) {

	}
};
struct CmpMeshRendererOpaque {
	std::shared_ptr<Material> material;
	std::shared_ptr<Mesh> mesh;
	uint64_t layers;

	CmpMeshRendererOpaque(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh, const uint64_t& layers = 1) : material(material), mesh(mesh), layers(layers) {

	}
};
struct CmpMeshRendererTransparent {
	std::shared_ptr<Material> material;
	std::shared_ptr<Mesh> mesh;
	uint64_t layers;

	CmpMeshRendererTransparent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh, const uint64_t& layers = 1) : material(material), mesh(mesh), layers(layers) {

	}
};
