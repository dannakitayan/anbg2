#pragma once
#include "BasicTypes.h"

class VBO
{
private:
	uint m_id = 0;
public:
	VBO();
	~VBO();

	void bind();
	void setData(const std::vector<GLfloat>& kVertices);
	void setData(GLfloat* p_data, size_t count);
	void unbind();

	//No copy;
	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;
};

