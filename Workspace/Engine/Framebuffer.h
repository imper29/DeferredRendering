#pragma once
#include <memory>
#include "Texture.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCube.h"
#include "TextureCubeArray.h"
#include "TextureStencilDepth.h"

enum FramebufferAttachment {
	Color0 = 0,
	Color1 = 1,
	Color2 = 2,
	Color3 = 3,
	Color4 = 4,
	Color5 = 5,
	Color6 = 6,
	Color7 = 7,

	Depth,
	Stencil,
	StencilDepth,
};
class Framebuffer {
public:
	Framebuffer();
	Framebuffer(int width, int height);
	~Framebuffer();

	int GetWidth() const;
	int GetHeight() const;
	GLuint GetFramebuffer() const;
	std::shared_ptr<Texture> GetTexture(FramebufferAttachment location) const;

	template<size_t size>
	void SetOutputs(const std::array<FramebufferAttachment, size>& outputs) {
		std::array<GLenum, size> outputsGl;
		for (size_t i = 0; i < size; ++i)
			outputsGl[i] = GetOpenGlAttachment(outputs[i]);
		glNamedFramebufferDrawBuffers(m_Framebuffer, size, &outputsGl[0]);
	}

	void Attach(FramebufferAttachment location, const std::shared_ptr<Texture2D>& texture, GLint mipmap = 0);
	void Attach(FramebufferAttachment location, const std::shared_ptr<Texture2DArray>& texture, GLint index = 0, GLint mipmap = 0);
	void Attach(FramebufferAttachment location, const std::shared_ptr<TextureCube>& texture, GLint side, GLint mipmap = 0);
	void Attach(FramebufferAttachment location, const std::shared_ptr<TextureCubeArray>& texture, GLint side, GLint index = 0, GLint mipmap = 0);
	void Attach(FramebufferAttachment location, const std::shared_ptr<TextureStencilDepth>& texture, GLint mipmap);

	static constexpr GLuint GetOpenGlAttachment(FramebufferAttachment attachment) {
		if (attachment == FramebufferAttachment::Depth) {
			return GL_DEPTH_ATTACHMENT;
		}
		else if (attachment == FramebufferAttachment::Stencil) {
			return GL_STENCIL_ATTACHMENT;
		}
		else if (attachment == FramebufferAttachment::StencilDepth) {
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		else {
			return GL_COLOR_ATTACHMENT0 + attachment;
		}
	}
private:
	void UpdateTextureReference(FramebufferAttachment location, const std::shared_ptr<Texture>& texture);

	GLuint m_Framebuffer;
	std::shared_ptr<Texture> m_Colors[8];
	std::shared_ptr<Texture> m_Stencil;
	std::shared_ptr<Texture> m_Depth;
	int m_Width, m_Height;
};
