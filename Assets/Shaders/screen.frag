#version 330 core
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// Texture from a framebuffer;
	vec4 albedo = screen_texture(texture0, texCoord);
	
	FragColor = vec4(albedo.rgb, albedo.a);
}