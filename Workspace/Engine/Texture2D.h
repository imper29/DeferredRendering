#pragma once
#include "Bitmap.h"
#include "Texture.h"

class Texture2D : public Texture {
public:
	Texture2D(const int32_t width, const int32_t height, GLenum format = GL_RGBA);
	Texture2D(const Bitmap& source);
};
