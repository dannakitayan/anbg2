#pragma once
#include "BasicTypes.h"

class EBO
{
private:
	uint m_id;
public:
	EBO();
	~EBO();
	void bind();
	void unbind();
	void setData(const std::vector<GLuint>& kIndexes);
	void setData(GLuint* p_data, size_t count);

	//No copy;
	EBO(const EBO&) = delete;
	EBO& operator=(const EBO&) = delete;
};