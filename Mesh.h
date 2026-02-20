#pragma once
#include "BasicTypes.h"

class Texture;
class Shader;
class EBO;
class VBO;
class VAO;

constexpr size_t kVerticesCount = 14;

class Mesh
{
private:
	glm::vec3 m_position{172, 144, 0};
	glm::mat4 m_model{};
	EBO* m_ebo = nullptr;
	VBO* m_vbo = nullptr;
	VAO* m_vao = nullptr;
	string m_texture_name{};

	int32 m_indexes_count = 0;
public:
	Mesh() = default;
	Mesh(const vector<GLfloat>& kVertices, const vector<GLuint>& kIndexes, const char* p_texture_name);
	~Mesh();
	VAO* getVAO() const;
	glm::mat4 getModel() const;
	int32 getIndexesCount() const;
};

