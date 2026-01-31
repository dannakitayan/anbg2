#pragma once
#include "BasicTypes.h"

enum class Folder : byte
{
	Textures,
	Sounds,
	Music,
	Sprites,
	Shaders,
	Models
};

enum class VertexAttribute : GLuint
{
	Position,
	Colors,
	UV,
	Normals
};

enum class TextureShaderType : byte
{
	Texture0,
	Texture1,
	NormalMap
};