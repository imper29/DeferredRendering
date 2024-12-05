#pragma once
#include <entt/entt.hpp>

struct CmpParent
{
	entt::entity parent;

	CmpParent(const entt::entity& parent) : parent(parent) {}
};
