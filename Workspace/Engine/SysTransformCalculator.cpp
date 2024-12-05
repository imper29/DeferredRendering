#include "SysTransformCalculator.h"

#include "CmpParent.h"
#include "CmpPosition.h"
#include "CmpRotation.h"
#include "CmpScale.h"
#include "CmpTransformLocalToWorld.h"

#include <map>
#include <vector>

struct transformHierarchyEntry {
	float4x4 matrix;
	entt::entity entity;
	size_t parent;

	transformHierarchyEntry(const float4x4& matrix, const entt::entity& entity, size_t parent) : matrix(matrix), entity(entity), parent(parent) {}
};
struct transformHierarchy {
	std::map<entt::entity, size_t> indicies;
	std::vector<transformHierarchyEntry> entries;

private:
	size_t includeEntity(entt::registry& entities, const entt::entity& entity, const float4x4& matrix) {
		//Include entity parent.
		auto parent = entities.try_get<CmpParent>(entity);
		auto parentIndex = SIZE_MAX;
		if (parent && entities.valid(parent->parent)) {
			auto parentMatrix = entities.try_get<CmpTransformLocalToWorld>(parent->parent);
			if (parentMatrix) {
				parentIndex = includeEntity(entities, parent->parent, parentMatrix->value);
			}
		}
		//Include entity.
		auto [emplacement, success] = indicies.try_emplace(entity, entries.size());
		if (success) {
			entries.push_back(transformHierarchyEntry(matrix, entity, parentIndex));
		}
		return emplacement->second;
	}
public:
	void refreshMatrices(entt::registry& entities) {
		for (auto& entry : entries) {
			if (entry.parent != SIZE_MAX) {
				entry.matrix = entries[entry.parent].matrix * entry.matrix;
			}
		}
		for (auto& entry : entries) {
			entities.get<CmpTransformLocalToWorld>(entry.entity).value = entry.matrix;
		}
	}
	transformHierarchy(entt::registry& entities) {
		auto entitiesProcessing = entities.view<CmpTransformLocalToWorld>();
		entries.reserve(entitiesProcessing.size());
		for (auto [entity, matrix] : entitiesProcessing.each()) {
			includeEntity(entities, entity, matrix.value);
		}
	}
};

SysTransformCalculator::SysTransformCalculator(Scene& scene) : SceneSystem(scene) {

}

void SysTransformCalculator::OnEvtReceived(Scene::EvtUpdate& evt) {
	//Generate local matricies in local to world component.
	{
		//3 of translation, rotation, and scale.
		for (auto [entity, localToWorld, position, rotation, scale] :
			scene.entities.view<CmpTransformLocalToWorld, CmpPosition, CmpRotation, CmpScale>().each()) {
			localToWorld.value = float4x4::translate(position.value) * float4x4::rotate(rotation.value) * float4x4::scale(scale.value);
		}
		//2 of translation, rotation, and scale.
		for (auto [entity, localToWorld, rotation, scale] :
			scene.entities.view<CmpTransformLocalToWorld, CmpRotation, CmpScale>(entt::exclude_t<CmpPosition>()).each()) {
			localToWorld.value = float4x4::rotate(rotation.value) * float4x4::scale(scale.value);
		}
		for (auto [entity, localToWorld, position, scale] :
			scene.entities.view<CmpTransformLocalToWorld, CmpPosition, CmpScale>(entt::exclude_t<CmpRotation>()).each()) {
			localToWorld.value = float4x4::translate(position.value) * float4x4::scale(scale.value);
		}
		for (auto [entity, localToWorld, position, rotation] :
			scene.entities.view<CmpTransformLocalToWorld, CmpPosition, CmpRotation>(entt::exclude_t<CmpScale>()).each()) {
			localToWorld.value = float4x4::translate(position.value) * float4x4::rotate(rotation.value);
		}
		//1 of translation, rotation, and scale.
		for (auto [entity, localToWorld, position] :
			scene.entities.view<CmpTransformLocalToWorld, CmpPosition>(entt::exclude_t<CmpRotation, CmpScale>()).each()) {
			localToWorld.value = float4x4::translate(position.value);
		}
		for (auto [entity, localToWorld, rotation] :
			scene.entities.view<CmpTransformLocalToWorld, CmpRotation>(entt::exclude_t<CmpPosition, CmpScale>()).each()) {
			localToWorld.value = float4x4::rotate(rotation.value);
		}
		for (auto [entity, localToWorld, scale] :
			scene.entities.view<CmpTransformLocalToWorld, CmpScale>(entt::exclude_t<CmpPosition, CmpRotation>()).each()) {
			localToWorld.value = float4x4::scale(scale.value);
		}
	}
	//Generate local to world matrices.
	transformHierarchy hierarchy = transformHierarchy(scene.entities);
	hierarchy.refreshMatrices(scene.entities);
}
