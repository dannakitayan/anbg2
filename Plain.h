#pragma once
#include "BasicTypes.h"

class EBO;
class VBO;
class VAO;
class Texture;
class Shader;

class Plain
{
private:
	glm::vec2 m_position{};
	glm::mat4 m_model{};
	GLfloat* m_vertices = nullptr;
	Texture* m_texture = nullptr;
	Shader* m_shader = nullptr;
	EBO* m_ebo = nullptr;
	VBO* m_vbo = nullptr;
	VAO* m_vao = nullptr;
public:
	Plain() = default;
	Plain(Shader* p_shader, Texture* p_texture);
	~Plain();

	Shader* getShader() const;
	Texture* getTexture() const;
	VAO* getVAO() const;
	glm::mat4 getModel() const;
	GLsizei getIndexCount() const { return 6; }
};

