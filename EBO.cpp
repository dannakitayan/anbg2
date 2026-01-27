#include "EBO.h"

EBO::EBO()
{
	glGenBuffers(1, &m_id);
}

EBO::~EBO()
{
	if (m_id) glDeleteBuffers(1, &m_id);
}

void EBO::setData(const std::vector<GLuint>& kIndexes)
{
	GLsizeiptr l_indexes_array_size = static_cast<GLsizeiptr>(kIndexes.size() * sizeof(GLuint));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l_indexes_array_size, kIndexes.data(), GL_STATIC_DRAW);
}

void EBO::setData(GLuint* p_data, size_t count)
{
	GLsizeiptr l_bytes = static_cast<GLsizeiptr>(count * sizeof(GLuint));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l_bytes, p_data, GL_STATIC_DRAW);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
