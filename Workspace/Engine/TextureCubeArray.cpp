#include "TextureCubeArray.h"

TextureCubeArray::TextureCubeArray(const int32_t width, const int32_t height, const int32_t depth) : Texture() {
	if (width <= 0)
		throw std::invalid_argument("Width must be positive.");
	if (height <= 0)
		throw std::invalid_argument("Height must be positive.");
	if (depth <= 0)
		throw std::invalid_argument("Depth must be positive.");
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_Texture);
	glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_RGBA, width, height, depth * 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}
TextureCubeArray::TextureCubeArray(const int32_t depth, const Bitmap* bitmaps) : Texture() {
	if (depth <= 0)
		throw std::invalid_argument("Depth must be positive.");
	//TODO: Ensure all textures are same size.
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_Texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_RGBA, bitmaps[0].width, bitmaps[0].height, depth * 6);
	for (int32_t i = 0; i < depth; ++i) {
		for (int32_t s = 0; s < 6; ++s) {
			int32_t l = 6 * i + s;
			glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, bitmaps[l].width, bitmaps[l].height, 1, GL_RGBA, GL_UNSIGNED_BYTE, bitmaps[l].pixels);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}
