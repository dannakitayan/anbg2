#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &m_id);
}

VBO::~VBO()
{
	if (m_id) glDeleteBuffers(1, &m_id);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::setData(const std::vector<GLfloat>& kVertices)
{
	GLsizeiptr l_vertices_array_size = static_cast<GLsizeiptr>(kVertices.size() * sizeof(GLfloat));
	glBufferData(GL_ARRAY_BUFFER, l_vertices_array_size, kVertices.data(), GL_STATIC_DRAW);
}

void VBO::setData(GLfloat* p_data, size_t count)
{
	GLsizeiptr l_bytes = static_cast<GLsizeiptr>(count * sizeof(GLfloat));
	glBufferData(GL_ARRAY_BUFFER, l_bytes, p_data, GL_STATIC_DRAW);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
