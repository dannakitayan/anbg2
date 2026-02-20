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
	Normals,
	Tangens,
	Frame
};

enum class TextureShaderType : byte
{
	Albedo,
	Normal,
	Specular,
	Environment,
	AORoughnessMetallic
};