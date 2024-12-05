#pragma once
#include <memory>
#include "Shader.h"
#include "float4x4.h"

class Material {
public:
	Material();
	virtual ~Material();
	virtual bool Use(const float4x4& projection, const float4x4& view, const float4x4& model) = 0;
};
