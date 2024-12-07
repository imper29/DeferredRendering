#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>

Mesh::Mesh(Mesh&& mesh) noexcept {
	m_Vao = mesh.m_Vao;
	m_Vbo = mesh.m_Vbo;
	m_DrawMode = mesh.m_DrawMode;
	m_VertexCount = mesh.m_VertexCount;

	mesh.m_Vao = 0;
	mesh.m_Vbo = 0;
	mesh.m_DrawMode = 0;
	mesh.m_VertexCount = 0;
}
Mesh& Mesh::operator=(Mesh&& mesh) noexcept {
	glDeleteVertexArrays(1, &m_Vao);
	glDeleteBuffers(1, &m_Vbo);

	m_Vao = mesh.m_Vao;
	m_Vbo = mesh.m_Vbo;
	m_DrawMode = mesh.m_DrawMode;
	m_VertexCount = mesh.m_VertexCount;

	mesh.m_Vao = 0;
	mesh.m_Vbo = 0;
	mesh.m_DrawMode = 0;
	mesh.m_VertexCount = 0;
	return *this;
}

Mesh::Mesh() {
	m_Vao = 0;
	m_Vbo = 0;
	m_DrawMode = 0;
	m_VertexCount = 0;
	m_Radius = 0;
}
Mesh::Mesh(const char* filename) : m_Vao(0), m_Vbo(0), m_Radius() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::vector<float3> positions;
	std::vector<float3> normals;
	std::vector<float2> uvs;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
		throw "The mesh could not be loaded!";
	}
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			float3 position;
			position.x = attrib.vertices[3 * index.vertex_index + 0];
			position.y = attrib.vertices[3 * index.vertex_index + 1];
			position.z = attrib.vertices[3 * index.vertex_index + 2];
			positions.push_back(position);

			float3 normal;
			normal.x = attrib.normals[3 * index.normal_index + 0];
			normal.y = attrib.normals[3 * index.normal_index + 1];
			normal.z = attrib.normals[3 * index.normal_index + 2];
			normals.push_back(normal);

			float2 uv;
			uv.x = attrib.texcoords[2 * index.texcoord_index + 0];
			uv.y = attrib.texcoords[2 * index.texcoord_index + 1];
			uvs.push_back(uv);
		}
	}

	std::vector<float3> tangents;
	tangents.reserve(normals.size());
	for (size_t i = 0; i < normals.size(); ++i) {
		float3 normal = normals[i];
		float3 c1 = normal.cross(float3(0.0f, 0.0f, 1.0f));
		float3 c2 = normal.cross(float3(0.0f, 1.0f, 0.0f));
		if (c1.lengthSq() > c2.lengthSq())
			tangents.push_back(c1.normalized());
		else
			tangents.push_back(c2.normalized());
	}
	new(this) Mesh(GL_TRIANGLES, (int32_t)positions.size(), &positions[0], &normals[0], &tangents[0], &uvs[0]);
}
Mesh::Mesh(GLenum drawMode, int32_t vertCount, const float3* vertPositions, const float3* vertNormals, const float3* vertTangents, const float2* vertUvs) {
#define ID_POSITIONS 0
#define POS_POSITIONS 0
#define LEN_POSITIONS (vertCount * sizeof(float3))

#define ID_NORMALS 1
#define POS_NORMALS POS_POSITIONS + LEN_POSITIONS
#define LEN_NORMALS (vertCount * sizeof(float3))

#define ID_TANGENTS 2
#define POS_TANGENTS POS_NORMALS + LEN_NORMALS
#define LEN_TANGENTS (vertCount * sizeof(float3))

#define ID_UVS 3
#define POS_UVS POS_TANGENTS + LEN_TANGENTS
#define LEN_UVS (vertCount * sizeof(float2))
	m_DrawMode = drawMode;
	m_VertexCount = vertCount;

	float radiusSquared = 0.0f;
	for (size_t i = 0; i < vertCount; ++i) {
		float radiusVert = vertPositions[i].lengthSq();
		if (radiusSquared < radiusVert)
			radiusSquared = radiusVert;
	}
	m_Radius = math::sqrt(radiusSquared);

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, LEN_POSITIONS + LEN_NORMALS + LEN_TANGENTS + LEN_UVS, NULL, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(ID_POSITIONS);
	glVertexAttribPointer(ID_POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_POSITIONS));
	glBufferSubData(GL_ARRAY_BUFFER, POS_POSITIONS, LEN_POSITIONS, vertPositions);

	glEnableVertexAttribArray(ID_NORMALS);
	glVertexAttribPointer(ID_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_NORMALS));
	glBufferSubData(GL_ARRAY_BUFFER, POS_NORMALS, LEN_NORMALS, vertNormals);

	glEnableVertexAttribArray(ID_TANGENTS);
	glVertexAttribPointer(ID_TANGENTS, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_TANGENTS));
	glBufferSubData(GL_ARRAY_BUFFER, POS_TANGENTS, LEN_TANGENTS, vertTangents);

	glEnableVertexAttribArray(ID_UVS);
	glVertexAttribPointer(ID_UVS, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_UVS));
	glBufferSubData(GL_ARRAY_BUFFER, POS_UVS, LEN_UVS, vertUvs);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Mesh::~Mesh() {
	if (m_Vao != 0) {
		glDeleteBuffers(1, &m_Vbo);
		glDeleteVertexArrays(1, &m_Vao);
	}
}

void Mesh::Draw(bool tess) const {
	Draw(tess ? GL_PATCHES : m_DrawMode);
}
void Mesh::Draw(GLenum drawMode) const {
	glBindVertexArray(m_Vao);
	glDrawArrays(drawMode, 0, m_VertexCount);
	glBindVertexArray(0);
}
void Mesh::DrawInstanced(int32_t count) const {
	DrawInstanced(count, m_DrawMode);
}
void Mesh::DrawInstanced(int32_t count, GLenum drawMode) const {
	glBindVertexArray(m_Vao);
	glDrawArraysInstanced(drawMode, 0, count, m_VertexCount);
	glBindVertexArray(0);
}

float Mesh::GetRadius() const
{
	return m_Radius;
}

GLuint Mesh::GetVao() const
{
	return m_Vao;
}
GLuint Mesh::GetVbo() const
{
	return m_Vbo;
}
GLenum Mesh::GetDrawMode() const
{
	return m_DrawMode;
}
int32_t Mesh::GetVertexCount() const
{
	return m_VertexCount;
}
