#include "Framebuffer.h"
#include "SceneManager.h"
#include "SDL.h"

Framebuffer::Framebuffer() : m_Width(), m_Height() {
	SDL_GetWindowSize(SceneManager::GetWindow(), &m_Width, &m_Height);
	glGenFramebuffers(1, &m_Framebuffer);
}
Framebuffer::Framebuffer(int width, int height) : m_Width(width), m_Height(height) {
	glGenFramebuffers(1, &m_Framebuffer);
}
Framebuffer::~Framebuffer() {
	if (m_Framebuffer) {
		glDeleteFramebuffers(1, &m_Framebuffer);
		m_Framebuffer = 0;
	}
}

GLuint Framebuffer::GetFramebuffer() const {
	return m_Framebuffer;
}

int Framebuffer::GetWidth() const {
	return m_Width;
}
int Framebuffer::GetHeight() const {
	return m_Height;
}
std::shared_ptr<Texture> Framebuffer::GetTexture(FramebufferAttachment location) const
{
	if (location == FramebufferAttachment::Depth) {
		return m_Depth;
	}
	else if (location == FramebufferAttachment::Stencil) {
		return m_Stencil;
	}
	else if (location == FramebufferAttachment::StencilDepth) {
		throw std::invalid_argument("The location cannot be stencil and depth when getting a texture from a framebuffer.");
	}
	else {
		return m_Colors[location];
	}
}

void Framebuffer::Attach(FramebufferAttachment location, const std::shared_ptr<Texture2D>& texture, GLint mipmap) {
	UpdateTextureReference(location, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GetOpenGlAttachment(location), GL_TEXTURE_2D, texture.get() ? texture->GetTexture() : 0, mipmap);
}
void Framebuffer::Attach(FramebufferAttachment location, const std::shared_ptr<Texture2DArray>& texture, GLint index, GLint mipmap) {
	UpdateTextureReference(location, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GetOpenGlAttachment(location), texture.get() ? texture->GetTexture() : 0, mipmap, index);
}
void Framebuffer::Attach(FramebufferAttachment location, const std::shared_ptr<TextureCube>& texture, GLint side, GLint mipmap) {
	UpdateTextureReference(location, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GetOpenGlAttachment(location), texture.get() ? texture->GetTexture() : 0, mipmap, side);
}
void Framebuffer::Attach(FramebufferAttachment location, const std::shared_ptr<TextureCubeArray>& texture, GLint side, GLint index, GLint mipmap) {
	UpdateTextureReference(location, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GetOpenGlAttachment(location), texture.get() ? texture->GetTexture() : 0, mipmap, side + index * 6);
}
void Framebuffer::Attach(FramebufferAttachment location, const std::shared_ptr<TextureStencilDepth>& texture, GLint mipmap) {
	UpdateTextureReference(location, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GetOpenGlAttachment(location), GL_TEXTURE_2D, texture.get() ? texture->GetTexture() : 0, mipmap);
}

void Framebuffer::UpdateTextureReference(FramebufferAttachment location, const std::shared_ptr<Texture>& texture) {
	if (location == FramebufferAttachment::Depth) {
		m_Depth = texture;
	}
	else if (location == FramebufferAttachment::Stencil) {
		m_Stencil = texture;
	}
	else if (location == FramebufferAttachment::StencilDepth) {
		m_Stencil = texture;
		m_Depth = texture;
	}
	else {
		m_Colors[location] = texture;
	}
}
