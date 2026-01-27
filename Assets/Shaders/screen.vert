#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 texCoord;

void main()
{
	texCoord = vec2(aUV.x, aUV.y);
}