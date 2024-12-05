#pragma once
#include "ShaderSource.h"
#include <GL/glew.h>
#include <initializer_list>

class Shader {
public:
	Shader(const Shader&) = delete;
	Shader& operator =(const Shader&) = delete;

	Shader(Shader&&) noexcept;
	Shader& operator =(Shader&&) noexcept;

	Shader();
	Shader(const std::initializer_list<ShaderSource>& sources);
	~Shader();

	bool Use() const;

	GLuint GetUniformLocation(const char* uniformName) const;
	GLuint GetProgram() const;
private:
	GLuint m_Program;
	bool m_Tessellated;
};
