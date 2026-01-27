#include "Texture.h"
#include "Shader.h"
#include <SDL_image.h>
#include "Path.h"
#include "Debug.h"
#include <fstream>

Texture::Texture()
{
	glGenTextures(1, &m_id);
}

Texture::~Texture()
{
	if(m_id) glDeleteTextures(1, &m_id);
}

uint Texture::getWidth() const
{
	return m_width;
}

uint Texture::getHeight() const
{
	return m_height;
}

void Texture::loadTexture(const char* p_texture_name, TextureShaderType type)
{
	if (!m_id) glGenTextures(1, &m_id);

	string l_texture_folder_path = getAssetPath(Folder::Textures, p_texture_name);
	if (l_texture_folder_path.empty())
	{
		debugLog("[Texture::loadTexture] invalid path to load a texture", p_texture_name);
		return;
	}
	debugLog("[Texture::loadTexture] The texture is loaded");
	SDL_Surface* l_p_surface = IMG_Load(l_texture_folder_path.c_str());

	if (!l_p_surface) {
		debugLog("[Texture::loadTexture] IMG_Load failed", IMG_GetError());
		return;
	}
	debugLog("[Texture::loadTexture] The surface is created");

	SDL_Surface* l_rgba_converted = SDL_ConvertSurfaceFormat(l_p_surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(l_p_surface);

	if (!l_rgba_converted) {
		debugLog("[Texture::loadTexture] ConvertSurfaceFormat failed", SDL_GetError());
		return;
	}
	l_p_surface = l_rgba_converted;
	debugLog("[Texture::loadTexture] ConvertSurfaceFormat is done");

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Filters set up;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_height = l_p_surface->h;
	m_width = l_p_surface->w;

	// Specifies the alignment requirements for the start of each pixel row in memory.
	// The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 
	// 4 (word-alignment), and 8 (rows start on double-word boundaries).
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, l_p_surface->w, l_p_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_p_surface->pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	debugLog("[Texture::loadTexture] glTexImage2D is done");

	m_texture_type = type;

	SDL_FreeSurface(l_p_surface);
}

void Texture::setTextureToShader(Shader* p_shader)
{
	p_shader->activateShader();

	GLuint l_unit = 0;
	GLint l_location = -1;

	switch (m_texture_type)
	{
	case TextureShaderType::Texture0:
		l_unit = 0;
		l_location = p_shader->texture0;
		break;
	case TextureShaderType::Texture1:
		l_unit = 1;
		l_location = p_shader->texture1;
		break;
	case TextureShaderType::NormalMap:
		l_unit = 2;
		l_location = p_shader->normal_map;
		break;
	default:
		return;
	}

	if (l_location != -1)
	{
		glUniform1i(l_location, l_unit);
		glActiveTexture(GL_TEXTURE0 + l_unit);
		glBindTexture(GL_TEXTURE_2D, m_id);
		debugLog("[Texture::setTextureToShader] texture is", std::to_string(l_unit).c_str());
		debugLog("[Texture::setTextureToShader] location is", std::to_string(l_location).c_str());
	}
	else
	{
		return;
	}
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}