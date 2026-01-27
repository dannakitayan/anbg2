#pragma once
#include "InputAction.h"

class Light;

class CameraMovement : public InputAction
{
private:
	Light* m_light;
public:
	// Inherited via InputAction
	void key_down_w() override;
	void key_down_a() override;
	void key_down_s() override;
	void key_down_d() override;
	void key_down_z() override;
	void key_down_x() override;
	CameraMovement() = default;
	~CameraMovement() = default;
	void setLight(Light* p_light);
};

