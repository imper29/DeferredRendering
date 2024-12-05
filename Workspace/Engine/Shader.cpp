#include "Shader.h"
#include "ShaderSource.h"
#include <fstream>
#include <iostream>

Shader::Shader(Shader&& program) noexcept {
	m_Program = program.m_Program;
	m_Tessellated = program.m_Tessellated;
	program.m_Program = 0;
	program.m_Tessellated = false;
}
Shader& Shader::operator=(Shader&& program) noexcept {
	if (m_Program) {
		glDeleteProgram(m_Program);
	}
	m_Program = program.m_Program;
	m_Tessellated = program.m_Tessellated;
	program.m_Program = 0;
	program.m_Tessellated = false;
	return *this;
}

Shader::Shader() {
	m_Program = 0;
	m_Tessellated = false;
}
Shader::Shader(const std::initializer_list<ShaderSource>& sources) {
	m_Program = glCreateProgram();
	m_Tessellated = false;
	for (auto& source : sources) {
		source.Attach(m_Program);
		if (source.GetStage() == ShaderStage::TessCtrl || source.GetStage() == ShaderStage::TessEval)
			m_Tessellated = true;
	}

	GLint status;
	glLinkProgram(m_Program);
	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	if (status == 0) {
		glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &status);
		std::string error;
		error.resize(status);
		glGetProgramInfoLog(m_Program, status, &status, &error[0]);
		std::cout << error << std::endl;
		throw error;
	}
}
Shader::~Shader() {
	if (m_Program) {
		glDeleteProgram(m_Program);
	}
}

GLuint Shader::GetUniformLocation(const char* uniformName) const
{
	return glGetUniformLocation(m_Program, uniformName);
}
GLuint Shader::GetProgram() const {
	return m_Program;
}
bool Shader::Use() const {
	if (m_Program) {
		glUseProgram(m_Program);
	}
	return m_Tessellated;
}
