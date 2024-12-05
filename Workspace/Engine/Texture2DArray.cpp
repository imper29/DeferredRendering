#include "Texture2DArray.h"

Texture2DArray::Texture2DArray(const int32_t width, const int32_t height, const int32_t depth) : Texture() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, width, height, depth);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
Texture2DArray::Texture2DArray(const int32_t depth, const Bitmap* bitmaps) : Texture() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, bitmaps[0].width, bitmaps[0].height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	for (int32_t i = 0; i < depth; ++i)
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, bitmaps[i].width, bitmaps[i].height, 1, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
