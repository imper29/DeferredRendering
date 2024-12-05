#include "TextureCube.h"

TextureCube::TextureCube(const Bitmap& xNeg, const Bitmap& xPos, const Bitmap& yNeg, const Bitmap& yPos, const Bitmap& zNeg, const Bitmap& zPos) : Texture() {
	//TODO: Ensure bitmap sizes are all positive and equal.
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xPos.width, xPos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xPos.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xNeg.width, xNeg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xNeg.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, yPos.width, yPos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, yPos.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yNeg.width, yNeg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, yNeg.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zPos.width, zPos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zPos.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zNeg.width, zNeg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zNeg.pixels);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
