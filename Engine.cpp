#include "Engine.h"
#include <SDL.h>
#include <glad/glad.h>
#include "Base.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	if (m_gl_context) SDL_GL_DeleteContext((SDL_GLContext)m_gl_context);
	if (m_window) SDL_DestroyWindow(m_window);
}

bool Engine::createWindow()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	SDL_Log("[Engine::createWindow] SDL_SetHint");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		SDL_Log("SDL error: %s", SDL_GetError());
		return false;
	}

	SDL_Log("[Engine::createWindow] SDL_Init");

	//Set up OpenGL version and a context;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_Log("[Engine::createWindow] Set up OpenGL version and a context");

	m_window = SDL_CreateWindow(g_game_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_windows_width, g_windows_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (m_window == nullptr)
	{
		SDL_Log("SDL create a window error: %s", SDL_GetError());
		return false;
	}

	SDL_Log("[Engine::createWindow] SDL_CreateWindow");

	m_gl_context = SDL_GL_CreateContext(m_window);
	if (!m_gl_context) {
		SDL_Log("OpenGL Context Error: %s", SDL_GetError());
		return false;
	}

	SDL_Log("[Engine::createWindow] SDL_GL_CreateContext");

	if (SDL_GL_MakeCurrent(m_window, m_gl_context) != 0) {
		SDL_Log("MakeCurrent failed: %s", SDL_GetError());
		return false;
	}

	SDL_Log("[Engine::createWindow] SDL_GL_MakeCurrent");

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to initialize GLAD");
		return false;
	}

	SDL_Log("[Engine::createWindow] gladLoadGLLoader");

	SDL_SetWindowBordered(m_window, SDL_TRUE);
	SDL_Log("[Engine::createWindow] SDL_SetWindowBordered");

	SDL_GetWindowSizeInPixels(m_window, &g_windows_width, &g_windows_height);
	SDL_Log("[Engine::createWindow] SDL_GetWindowSizeInPixels");

	glViewport(0, 0, g_screen_width, g_screen_height);
	SDL_Log("[Engine::createWindow] glViewport");

	SDL_ShowWindow(m_window);
	SDL_Log("[Engine::createWindow] SDL_ShowWindow");

	return true;
}

void Engine::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Engine::vsync(bool state)
{
	SDL_GL_SetSwapInterval(state);
}

void Engine::alphaBlend(bool state)
{
	if (state)
	{
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		//glDisable(GL_BLEND);
	}
}

void Engine::cleanScreen()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
