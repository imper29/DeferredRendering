#pragma once
#include <GL/glew.h>

class Texture {
public:
	Texture();
	Texture(GLuint texture, bool owning);
	virtual ~Texture();

	GLuint GetTexture() const;
protected:
	GLuint m_Texture;
private:
	bool m_Owning;
};
