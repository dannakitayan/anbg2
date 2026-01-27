#pragma once

class Mesh;

class Render
{
private:
public:
	Render();
	~Render();
	void drawMesh(Mesh* const p_mesh);
};

