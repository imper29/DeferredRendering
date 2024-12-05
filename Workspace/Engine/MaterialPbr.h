#pragma once
#include <memory>
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class MaterialPbr : public Material {
public:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> emission;
	std::shared_ptr<Texture> occlusionRoughnessMetalness;
	std::shared_ptr<Texture> displacement;
	std::shared_ptr<Texture> reflection;
	float displacementScale;

	MaterialPbr(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& albedo, const std::shared_ptr<Texture>& normal, const std::shared_ptr<Texture>& emission, const std::shared_ptr<Texture>& occlusionRoughnessMetalness, const std::shared_ptr<Texture>& displacement = nullptr, const std::shared_ptr<Texture>& reflection = nullptr, const float displacementScale = 0.0f);
	~MaterialPbr() override;
	bool Use(const float4x4& projection, const float4x4& view, const float4x4& model) override;
};
