#pragma once
class InputAction
{
protected:
	//If keys pressed;
	bool m_is_pressed = false;

	//If keys released;
	bool m_w_key_released = true;
	bool m_a_key_released = true;
	bool m_s_key_released = true;
	bool m_d_key_released = true;
public:
	virtual void key_down_w() = 0;
	virtual void key_down_a() = 0;
	virtual void key_down_s() = 0;
	virtual void key_down_d() = 0;
	virtual void key_down_z() = 0;
	virtual void key_down_x() = 0;

	//virtual void key_up_w() = 0;
	//virtual void key_up_a() = 0;
	//virtual void key_up_s() = 0;
	//virtual void key_up_d() = 0;
	//virtual void key_up_escape() = 0;
	//virtual void key_up_spacebar() = 0;

	InputAction();
	~InputAction();
};