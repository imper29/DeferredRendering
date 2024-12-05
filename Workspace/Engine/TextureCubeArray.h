#pragma once
#include "Bitmap.h"
#include "Texture.h"

class TextureCubeArray : public Texture {
public:
	TextureCubeArray(const int32_t width, const int32_t height, const int32_t depth);
	TextureCubeArray(const int32_t depth, const Bitmap* bitmaps);
};
