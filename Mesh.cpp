#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture.h"
#include "Debug.h"

//Mesh::Mesh(const char* p_shader_name, const char* p_texture_name)
//{
//
//}

Mesh::Mesh(Shader* p_shader, Texture* p_texture)
{
	m_model = glm::mat4(1.0f);
	debugLog("[Mesh::Mesh] Set m_model matrix");

	m_vao = new VAO;
	debugLog("[Mesh::Mesh] VAO is created");

	m_vbo = new VBO;
	debugLog("[Mesh::Mesh] VBO is created");

	m_ebo = new EBO;
	debugLog("[Mesh::Mesh] EBO is created");

	m_texture = p_texture;
	m_shader = p_shader;
	debugLog("[Mesh::Mesh] Texture and Shader pointers have gotten");

	//VAO;
	m_vao->bind();
	debugLog("[Mesh::Mesh] m_vao->bind()");

	//VBO;
	m_vbo->bind();
	debugLog("[Mesh::Mesh] m_vao->bind()");

	GLfloat l_uv_0[2] = { 0.0f, 0.0f };
	GLfloat l_uv_1[2] = { 1.0f, 1.0f };

	GLfloat l_vertices[] =
	{
		//aPos													//aColor			//aUV					//aNormal
		0.0f,					 0.0f,				   0.0f,	1.0f, 1.0f, 1.0f,	l_uv_0[0], l_uv_0[1],	0.0f, 0.0f, 1.0f,
		p_texture->getWidth(),			 0.0f,		   0.0f,	1.0f, 1.0f, 1.0f,	l_uv_1[0], l_uv_0[1],	0.0f, 0.0f, 1.0f,
		0.0f,		p_texture->getHeight(),			   0.0f,	1.0f, 1.0f, 1.0f,	l_uv_0[0], l_uv_1[1],	0.0f, 0.0f, 1.0f,
		p_texture->getWidth(), p_texture->getHeight(), 0.0f,	1.0f, 1.0f, 1.0f,	l_uv_1[0], l_uv_1[1],	0.0f, 0.0f, 1.0f
	};

	m_vbo->setData(l_vertices, 44);
	debugLog("[Mesh::Mesh] m_vbo->setData");

	//EBO;
	m_ebo->bind();
	debugLog("[Mesh::Mesh] m_ebo->bind()");

	GLuint l_indexes[] =
	{
		2, 0, 1, // CAB
		2, 1, 3, // CBD
	};

	m_ebo->setData(l_indexes, 6);
	debugLog("[Mesh::Mesh] m_ebo->setData");

	//VAO; BindAttributes;
	const GLsizei stride = 11 * sizeof(GLfloat);
	m_vao->bindAttributes(VertexAttribute::Position, 3, stride, 0);//aPos;
	m_vao->bindAttributes(VertexAttribute::Colors, 3, stride, 3 * sizeof(GLfloat));//aColors;
	m_vao->bindAttributes(VertexAttribute::UV, 2, stride, 6 * sizeof(GLfloat));//aUV;
	m_vao->bindAttributes(VertexAttribute::Normals, 3, stride, 8 * sizeof(GLfloat));//aNormal;
	debugLog("[Mesh::Mesh] All attributes are binded");
}

Mesh::~Mesh()
{
	delete(m_vao);
	delete(m_vbo);
	delete(m_ebo);
}

Shader* Mesh::getShader() const
{
	return m_shader;
}

Texture* Mesh::getTexture() const
{
	return m_texture;
}

VAO* Mesh::getVAO() const
{
	return m_vao;
}

glm::mat4 Mesh::getModel() const
{
	return m_model;

}