#pragma once
#include "BasicTypes.h"

class EBO;
class VBO;
class VAO;
class Texture;
class Shader;

class Mesh
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
	Mesh() = default;
	Mesh(Shader* p_shader, Texture* p_texture);
	~Mesh();

	Shader* getShader() const;
	Texture* getTexture() const;
	VAO* getVAO() const;
	glm::mat4 getModel() const;
	GLsizei getIndexCount() const { return 6; }
};

