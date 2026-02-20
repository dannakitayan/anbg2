#pragma once
#include "BasicTypes.h"

class TBO
{
private:
	uint m_id = 0;
	uint m_texture_id = 0;
public:
	TBO();
	~TBO();

	void bind();

	TBO(const TBO&) = delete;
	TBO& operator=(const TBO&) = delete;
};

