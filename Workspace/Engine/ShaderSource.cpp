#include "ShaderSource.h"
#include <fstream>
#include <memory>
#include <iostream>

static GLuint GetGlStage(ShaderStage stage) {
	if (stage == ShaderStage::TessCtrl)
		return GL_TESS_CONTROL_SHADER;
	if (stage == ShaderStage::TessEval)
		return GL_TESS_EVALUATION_SHADER;
	if (stage == ShaderStage::Vert)
		return GL_VERTEX_SHADER;
	if (stage == ShaderStage::Geom)
		return GL_GEOMETRY_SHADER;
	if (stage == ShaderStage::Frag)
		return GL_FRAGMENT_SHADER;
	if (stage == ShaderStage::Comp)
		return GL_COMPUTE_SHADER;
	throw "Unknown shader stage!";
}

ShaderSource::ShaderSource() : m_Stage(ShaderStage::None), m_Shader(0) {

}
ShaderSource::ShaderSource(ShaderSource&& source) noexcept {
	m_Stage = source.m_Stage;
	m_Shader = source.m_Shader;
	source.m_Stage = ShaderStage::None;
	source.m_Shader = 0;
}
ShaderSource::ShaderSource(const char* source, ShaderStage stage) : m_Stage(stage) {
	m_Shader = glCreateShader(GetGlStage(stage));
	glShaderSource(m_Shader, 1, &source, 0);
	glCompileShader(m_Shader);

	GLint status;
	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &status);
		std::string error;
		error.resize(status + 1);
		error[status] = '\0';
		glGetShaderInfoLog(m_Shader, status, &status, &error[0]);
		glDeleteShader(m_Shader);
		throw error;
	}
}
ShaderSource::~ShaderSource() {
	if (m_Shader != 0) {
		glDeleteShader(m_Shader);
	}
}

ShaderStage ShaderSource::GetStage() const {
	return m_Stage;
}
ShaderSource ShaderSource::FromFile(const char* source, ShaderStage stage) {
	std::ifstream stream;
	stream.open(source);
	stream.seekg(0, std::ios::end);
	size_t length = stream.tellg();
	stream.seekg(0, std::ios::beg);
	auto buffer = new char[length + 1];
	buffer[length] = '\0';
	stream.read(buffer, length);
	stream.close();
	try {
		ShaderSource result = ShaderSource::FromString(buffer, stage);
		delete[] buffer;
		return result;
	}
	catch (std::string& error) {
		delete[] buffer;
		std::cout << source << std::endl;
		std::cout << error << std::endl;
		throw error;
	}
}
ShaderSource ShaderSource::FromString(const char* source, ShaderStage stage) {
	return ShaderSource(source, stage);
}

void ShaderSource::Attach(GLint program) const {
	if (m_Shader != 0) {
		glAttachShader(program, m_Shader);
	}
}
