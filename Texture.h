#pragma once
#include "EngineTypes.h"

class Shader;

class Texture
{
private:
	uint m_width;
	uint m_height;
	GLuint m_id;
	GLenum m_type;
	TextureShaderType m_texture_type = TextureShaderType::Albedo;
public:
	Texture();
	~Texture();
	uint getWidth() const;
	uint getHeight() const;

	void loadTexture(const char* p_texture_name, TextureShaderType type);
	void setTextureToShader(Shader* p_shader);
	void unbind();
	GLuint getId() const { return m_id; }
};

