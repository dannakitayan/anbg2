#pragma once
#include "BasicTypes.h"
#include <array>

class Mesh;
class Texture;
class Shader;

class Render
{
private:
public:
	Render();
	~Render();
	void drawMesh(const Mesh* kModel, std::array<Texture*, 5>& textures, Shader* p_shader);
};

