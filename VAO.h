#pragma once
#include "EngineTypes.h"

class VAO
{
private:
	uint m_id;
public:
	VAO();
	~VAO();

	void bindAttributes(VertexAttribute location, GLint values_per_vertex, GLsizei stride, size_t offset);
	void bind();
	void unbind();

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;
};

