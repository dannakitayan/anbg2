#include "Light.h"

glm::vec3 Light::getColor() const
{
    return m_color;
}

glm::vec3 Light::getPosition() const
{
    return m_position;
}

float Light::getIntensity() const
{
    return m_intensity;
}

float Light::getRadius() const
{
    return m_radius;
}

void Light::setColor(const glm::vec3& kColor)
{
    m_color = kColor;
}

void Light::setPostion(const glm::vec2& kPosition)
{
    m_position.x = kPosition.x;
    m_position.y = kPosition.y;
}

void Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

void Light::setRadius(float radius)
{
    m_radius = radius * kRadiusAddionalValue;
}

void Light::setX(float x)
{
    m_position.x += x;
}

void Light::setY(float y)
{
    m_position.y += y;
}

void Light::setZ(float z)
{
    m_position.z += z;
}
