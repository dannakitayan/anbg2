#include "Input.h"
#include "InputAction.h"
#include <SDL.h>

Input::Input()
{
	m_event = new SDL_Event{};
}

Input::~Input()
{
	delete(m_event);
}

void Input::processAction(InputAction* const p_kInputAction)
{
	SDL_Event* l_p_current = static_cast<SDL_Event*>(m_event);
	while (SDL_PollEvent(l_p_current))
	{
		switch (l_p_current->type)
		{
		case SDL_KEYDOWN:
		{
			switch (l_p_current->key.keysym.sym)
			{
			case SDLK_w:
				p_kInputAction->key_down_w();
				break;
			case SDLK_a:
				p_kInputAction->key_down_a();
				break;
			case SDLK_s:
				p_kInputAction->key_down_s();
				break;
			case SDLK_d:
				p_kInputAction->key_down_d();
				break;
			case SDLK_x:
				p_kInputAction->key_down_x();
				break;
			case SDLK_z:
				p_kInputAction->key_down_z();
				break;
			default: break;
			}
			break;
		}
		//case SDL_KEYUP:
		//{
		//	switch (m_event.key.keysym.sym)
		//	{
		//	case SDLK_w:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_w();
		//		}
		//		break;
		//	case SDLK_a:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_a();
		//		}
		//		break;
		//	case SDLK_s:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_s();
		//		}
		//		break;
		//	case SDLK_d:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_d();
		//		}
		//		break;
		//	case SDLK_SPACE:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_spacebar();
		//		}
		//		break;
		//	case SDLK_ESCAPE:
		//		for (auto& l_actions : m_actions)
		//		{
		//			l_actions->key_up_a();
		//		}
		//		break;
		//	default: break;
		//	}
		//	break;
		//}
		default:
			break;
		}
	}
}
