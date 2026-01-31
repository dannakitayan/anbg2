#include "Camera.h"
#include "Base.h"
#include <gtc/matrix_transform.hpp>
#include "Shader.h";
#include <gtc/type_ptr.hpp>

Camera::Camera()
{
	m_projection = glm::ortho(0.0f, (float)g_windows_width, (float)g_windows_height, 0.0f, -100.0f, 1000.0f);
	m_view = glm::translate(
		glm::mat4(1.0f),                    // identity
		glm::vec3(-m_position[0], -m_position[1], 0.0f)
	);
}

Camera::~Camera()
{
}

void Camera::setCameraToShader(Shader* const p_shader)
{
	p_shader->activateShader();

	if (p_shader->view != -1)
		glUniformMatrix4fv(p_shader->view, 1, GL_FALSE,
			glm::value_ptr(m_view));

	if (p_shader->projection != -1)
		glUniformMatrix4fv(p_shader->projection, 1, GL_FALSE,
			glm::value_ptr(m_projection));
}

void Camera::setCameraPosition(const glm::vec2& kPosition)
{
	m_position[0] += kPosition[0];
	m_position[1] += kPosition[1];
	updateView();
}

void Camera::updateView()
{
	m_view = glm::translate(
		glm::mat4(1.0f),                
		glm::vec3(-m_position[0], -m_position[1], 0.0f)
	);
}
