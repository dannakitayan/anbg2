#include "Debug.h"
#include <SDL.h>

void debugLog(const char* p_message)
{
	SDL_Log(p_message);
}

void debugLog(const char* p_message, const int32& value)
{
	string l_message = std::string(p_message) + ": " + std::to_string(value);
	SDL_Log(l_message.c_str());
}

void debugLog(const char* p_message, const char* p_add)
{
	string l_message = std::string(p_message) + ": " + std::string(p_add);
	SDL_Log(l_message.c_str());
}
