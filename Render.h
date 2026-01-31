#pragma once
#include "BasicTypes.h"

class Mesh;
class Texture;
class Shader;

class Render
{
private:
public:
	Render();
	~Render();
	void drawMesh(const vector<Mesh*>& kModels, Texture* p_texture, Shader* p_shader);
};

