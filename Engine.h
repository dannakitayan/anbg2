#pragma once

struct SDL_Window;

class Engine
{
private:
	SDL_Window* m_window = nullptr;
	void* m_gl_context = nullptr;
public:
	Engine();
	~Engine();
	bool createWindow();
	void swapBuffers();
	void vsync(bool state);
	void alphaBlend(bool state);
	void cleanScreen(void);
};

