#pragma once
#include "gl/glew.h"

enum class ShaderStage {
	None,
	TessCtrl,
	TessEval,
	Vert,
	Geom,
	Frag,
	Comp,
};

class Shader;
class ShaderSource {
	friend Shader;
public:
	ShaderSource();
	ShaderSource(ShaderSource&& source) noexcept;
	ShaderSource(const char* source, ShaderStage stage);
	~ShaderSource();

public:
	ShaderStage GetStage() const;
	static ShaderSource FromFile(const char* source, ShaderStage stage);
	static ShaderSource FromString(const char* source, ShaderStage stage);
private:
	void Attach(GLint program) const;

	ShaderStage m_Stage;
	GLint m_Shader;
};
