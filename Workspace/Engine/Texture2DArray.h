#pragma once
#include "Bitmap.h"
#include "Texture.h"

class Texture2DArray : public Texture {
public:
	Texture2DArray(const int32_t width, const int32_t height, const int32_t depth);
	Texture2DArray(const int32_t depth, const Bitmap* bitmaps);
};
