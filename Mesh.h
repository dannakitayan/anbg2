#pragma once
#include "BasicTypes.h"

class Texture;
class Shader;
class EBO;
class VBO;
class VAO;

class Mesh
{
private:
	glm::vec3 m_position{};
	glm::mat4 m_model{};
	//GLfloat* m_vertices = nullptr;
	//Texture* m_texture = nullptr;
	//Shader* m_shader = nullptr;
	EBO* m_ebo = nullptr;
	VBO* m_vbo = nullptr;
	VAO* m_vao = nullptr;
	string m_texture_name{};
public:
	Mesh() = default;
	Mesh(const vector<GLfloat>& kVertices, const vector<GLuint>& kIndexes, const char* p_texture_name);
	~Mesh();
};

