#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;
out vec3 vertexColor;
out vec2 texCoord;
out float timeValue;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
	timeValue = time;
	vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertexColor = aColor;
	//For 2D game;
	texCoord = vec2(aUV.x, aUV.y);
}