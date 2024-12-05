#include "Texture.h"
#include <iostream>

Texture::Texture() {
	m_Owning = true;
	glGenTextures(1, &m_Texture);
}
Texture::Texture(GLuint texture, bool owning) : m_Texture(texture), m_Owning(owning) {

}
Texture::~Texture() {
	if (m_Owning && m_Texture) {
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}
}

GLuint Texture::GetTexture() const {
	return m_Texture;
}
