#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_id);
}

VAO::~VAO()
{
	if(m_id) glDeleteVertexArrays(1, &m_id);
}

void VAO::bindAttributes(VertexAttribute location, GLint values_per_vertex, GLsizei stride, size_t offset)
{
	GLuint l_index = static_cast<GLuint>(location);
	glVertexAttribPointer(l_index, values_per_vertex, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
	glEnableVertexAttribArray(l_index);
}

void VAO::bind()
{
	glBindVertexArray(m_id);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}
