#pragma once
#include "Bitmap.h"
#include "Texture.h"

class TextureCube : public Texture {
public:
	TextureCube(const Bitmap& xNeg, const Bitmap& xPos, const Bitmap& yNeg, const Bitmap& yPos, const Bitmap& zNeg, const Bitmap& zPos);
};
