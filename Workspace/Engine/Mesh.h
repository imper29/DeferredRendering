#pragma once
#include <GL/glew.h>
#include "float3.h"

class Mesh {
public:
	Mesh(const Mesh&) = delete;
	Mesh& operator =(const Mesh&) = delete;

	Mesh(Mesh&&) noexcept;
	Mesh& operator =(Mesh&&) noexcept;

	Mesh();
	Mesh(const char* filename);
	Mesh(GLenum drawMode, int32_t vertCount, const float3* vertPositions, const float3* vertNormals, const float3* vertTangents, const float2* vertUvs);
	~Mesh();

	void Draw(bool tess) const;
	void Draw(GLenum drawMode) const;
	void DrawInstanced(int32_t count) const;
	void DrawInstanced(int32_t count, GLenum drawMode) const;

	GLuint GetVao() const;
	GLuint GetVbo() const;
	GLenum GetDrawMode() const;
	int32_t GetVertexCount() const;
private:
	GLuint m_Vao, m_Vbo;
	GLenum m_DrawMode;
	GLenum m_DrawModeTess;
	int32_t m_VertexCount;
};
