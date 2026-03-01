#include "CameraMovement.h"
#include "Camera.h"
#include "Debug.h"

void CameraMovement::key_down_w()
{
	m_camera->setCameraPosition(glm::vec2(0.0, -1.0));
}

void CameraMovement::key_down_a()
{
	m_camera->setCameraPosition(glm::vec2(-1.0, 0.0));
}

void CameraMovement::key_down_s()
{
	m_camera->setCameraPosition(glm::vec2(0.0, 1.0));
}

void CameraMovement::key_down_d()
{
	m_camera->setCameraPosition(glm::vec2(1.0, 0.0));
}

void CameraMovement::key_down_z()
{
	
}

void CameraMovement::key_down_x()
{
	
}

void CameraMovement::setCamera(Camera* p_camera)
{
	m_camera = p_camera;
}