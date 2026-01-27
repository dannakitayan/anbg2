#include "FBO.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Shader.h"

FBO::FBO()
{
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

FBO::~FBO()
{
	unbind();
}

void FBO::generateFrameBuffer(uint width, uint height, Shader* p_shader)
{
	glGenTextures(1, &m_famebuffer_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_famebuffer_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_famebuffer_texture_id);

	p_shader->
}

void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glBindTexture(GL_TEXTURE_2D, m_famebuffer_texture_id);
}

void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
