#pragma once
#include "EngineTypes.h"

class InputAction;

class Input
{
private:
	void* m_event;
public:
	Input();
	~Input();
	void processAction(InputAction* const p_kInputAction);
};