#include "CameraMovement.h"
#include "Light.h"

void CameraMovement::key_down_w()
{
	m_light->setY(-1.0);
}

void CameraMovement::key_down_a()
{
	m_light->setX(-1.0);
}

void CameraMovement::key_down_s()
{
	m_light->setY(1.0);
}

void CameraMovement::key_down_d()
{
	m_light->setX(1.0);
}

void CameraMovement::key_down_z()
{
	m_light->setZ(1.0);
}

void CameraMovement::key_down_x()
{
	m_light->setZ(-1.0);
}

void CameraMovement::setLight(Light* p_light)
{
	m_light = p_light;
}