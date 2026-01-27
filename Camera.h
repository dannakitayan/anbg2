#pragma once
#include "BasicTypes.h"

class Shader;

class Camera
{
private:
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::vec2 m_position{};
public:
	Camera();
	~Camera();
	void setCameraToShader(Shader* const p_shader);
	void setCameraPosition(const glm::vec2& kPosition);
	void updateView();
};

