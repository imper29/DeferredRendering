#include "MaterialPbr.h"

MaterialPbr::MaterialPbr(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& albedo, const std::shared_ptr<Texture>& normal, const std::shared_ptr<Texture>& emission, const std::shared_ptr<Texture>& occlusionRoughnessMetalness, const std::shared_ptr<Texture>& displacement, const std::shared_ptr<Texture>& reflection, const float displacementScale)
	: shader(shader), albedo(albedo), normal(normal), emission(emission), occlusionRoughnessMetalness(occlusionRoughnessMetalness), displacement(displacement), reflection(reflection), displacementScale(displacementScale) {

}
MaterialPbr::~MaterialPbr() {

}
bool MaterialPbr::Use(const float4x4& projection, const float4x4& view, const float4x4& model) {
	auto result = shader->Use();

	glBindTextureUnit(0, albedo ? albedo->GetTexture() : 0);
	glBindTextureUnit(1, normal ? normal->GetTexture() : 0);
	glBindTextureUnit(2, emission ? emission->GetTexture() : 0);
	glBindTextureUnit(3, occlusionRoughnessMetalness ? occlusionRoughnessMetalness->GetTexture() : 0);
	glBindTextureUnit(4, displacement ? displacement->GetTexture() : 0);
	glBindTextureUnit(5, reflection ? reflection->GetTexture() : 0);

	glUniform1i(shader->GetUniformLocation("albedoMap"), 0);
	glUniform1i(shader->GetUniformLocation("normalMap"), 1);
	glUniform1i(shader->GetUniformLocation("emissionMap"), 2);
	glUniform1i(shader->GetUniformLocation("occlusionRoughnessMetalnessMap"), 3);
	glUniform1i(shader->GetUniformLocation("displacementMap"), 4);
	glUniform1i(shader->GetUniformLocation("reflectionMap"), 5);

	glUniform1f(shader->GetUniformLocation("displacementScale"), displacementScale);

	glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, false, projection);
	glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, false, view);
	glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, false, model);

	return result;
}
