#include "CameraMovement.h"
#include "Light.h"
#include "Debug.h"

void CameraMovement::key_down_w()
{
	m_light->setY(-1.0);
	debugLog("Light y position", std::to_string(m_light->getPosition().y).c_str());
}

void CameraMovement::key_down_a()
{
	m_light->setX(-1.0);
	debugLog("Light x position", std::to_string(m_light->getPosition().x).c_str());
}

void CameraMovement::key_down_s()
{
	m_light->setY(1.0);
	debugLog("Light y position", std::to_string(m_light->getPosition().y).c_str());
}

void CameraMovement::key_down_d()
{
	m_light->setX(1.0);
	debugLog("Light x position", std::to_string(m_light->getPosition().x).c_str());
}

void CameraMovement::key_down_z()
{
	m_light->setZ(1.0);
	debugLog("Light z position", std::to_string(m_light->getPosition().z).c_str());
}

void CameraMovement::key_down_x()
{
	m_light->setZ(-1.0);
	debugLog("Light z position", std::to_string(m_light->getPosition().z).c_str());
}

void CameraMovement::setLight(Light* p_light)
{
	m_light = p_light;
}