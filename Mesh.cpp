#include "Mesh.h"
#include "Debug.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

Mesh::Mesh(const vector<GLfloat>& kVertices, const vector<GLuint>& kIndexes, const char* p_texture_name)
{
	if (kVertices.size() == 0)
	{
		debugLog("[Mesh::Mesh] No vertices");
		return;
	}

	if (kIndexes.size() == 0)
	{
		debugLog("[Mesh::Mesh] No indexes");
		return;
	}

	if (!p_texture_name || !p_texture_name[0]) {
		debugLog("[Mesh::Mesh] No texture");
		return;
	}

	m_model = glm::mat4(1.0f);
	debugLog("[Mesh::Mesh] Set m_model matrix");

	m_vao = new VAO;
	debugLog("[Mesh::Mesh] VAO is created");

	m_vbo = new VBO;
	debugLog("[Mesh::Mesh] VBO is created");

	m_ebo = new EBO;
	debugLog("[Mesh::Mesh] EBO is created");

	//VAO;
	m_vao->bind();
	debugLog("[Mesh::Mesh] m_vao->bind()");

	//VBO;
	m_vbo->bind();
	debugLog("[Mesh::Mesh] m_vao->bind()");

	m_vbo->setData(kVertices);
	debugLog("[Mesh::Mesh] m_vbo->setData");

	//EBO;
	m_ebo->bind();
	debugLog("[Mesh::Mesh] m_ebo->bind()");

	m_ebo->setData(kIndexes);
	debugLog("[Mesh::Mesh] m_ebo->setData");

	//VAO; BindAttributes;
	const GLsizei stride = kVerticesCount * sizeof(GLfloat);
	m_vao->bindAttributes(VertexAttribute::Position, 3, stride, 0);//aPos;
	m_vao->bindAttributes(VertexAttribute::Colors, 3, stride, 3 * sizeof(GLfloat)); //aColors;
	m_vao->bindAttributes(VertexAttribute::UV, 2, stride, 6 * sizeof(GLfloat)); //aUV;
	m_vao->bindAttributes(VertexAttribute::Normals, 3, stride, 8 * sizeof(GLfloat)); //aNormal;
	m_vao->bindAttributes(VertexAttribute::Tangens, 3, stride, 11 * sizeof(GLfloat)); //aTangent;
	debugLog("[Mesh::Mesh] All attributes are binded");

	m_texture_name = string(p_texture_name);
	debugLog("[Mesh::Mesh] Texture is assigned");

	m_indexes_count = kIndexes.size();
	debugLog("[Mesh::Mesh] Mesh has count of indexes", std::to_string(m_indexes_count).c_str());
	debugLog("[Mesh::Mesh] Mesh has count of vertices", std::to_string(kVertices.size()).c_str());
}

Mesh::~Mesh()
{
	delete(m_vao);
	delete(m_vbo);
	delete(m_ebo);
}

VAO* Mesh::getVAO() const
{
	return m_vao;
}

glm::mat4 Mesh::getModel() const
{
	return m_model;

}

int32 Mesh::getIndexesCount() const
{
	return m_indexes_count;
}
