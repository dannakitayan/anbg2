#pragma once
#include "BasicTypes.h"

class Light
{
private:
	const byte kRadiusAddionalValue = 10;
	glm::vec3 m_color{ 1.0f, 0.8f, 0.4f };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0 };
	float m_intensity = 0.0f;
	float m_radius = 0.0f;
public:
	Light() = default;
	~Light() = default;

	glm::vec3 getColor() const;
	glm::vec3 getPosition() const;
	float getIntensity() const;
	float getRadius() const;
	void setColor(const glm::vec3& kColor);
	void setPostion(const glm::vec2& kPosition);
	void setIntensity(float intensity);
	void setRadius(float radius);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
};

