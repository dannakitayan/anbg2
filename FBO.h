#pragma once
#include "EngineTypes.h"

class EBO;
class VBO;
class VAO;
class Shader;

class FBO
{
private:
	GLuint m_id = 0;
	GLuint m_famebuffer_texture_id = 0;

	EBO* m_ebo = nullptr;
	VBO* m_vbo = nullptr;
	VAO* m_vao = nullptr;
public:
	FBO();
	~FBO();
	void generateFrameBuffer(uint width, uint height, Shader* p_shader);
	void bind();
	void unbind();
};

