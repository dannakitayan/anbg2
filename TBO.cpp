#include "TBO.h"

TBO::TBO()
{
	glGenBuffers(1, &m_id);
	glGenTextures(1, &m_texture_id);
}

TBO::~TBO()
{
}

void TBO::bind()
{
	glBindBuffer(GL_TEXTURE_BUFFER, m_id);
	//glBufferData(GL_TEXTURE_BUFFER, dataSize, data, GL_STATIC_DRAW);
}
