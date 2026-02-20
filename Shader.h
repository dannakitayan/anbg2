#pragma once
#include "BasicTypes.h"

class Light;

class Shader
{
private:
	GLuint m_program_id = 0;
	string m_vertex_shader{};
	string m_fragment_shader{};
	GLuint m_vertex_id = 0;
	GLuint m_fragment_id = 0;
public:
	//Uniforms;
	GLuint model;
	GLuint view;
	GLuint projection;
	GLuint albedo;
	GLuint normal_map;
	GLuint specular_map;
	GLuint environment_map;
	GLuint glossiness_metallic;
	GLuint time;

	Shader();
	~Shader();
	void loadShader(const char* p_vertex_shader_name, const char* p_fragment_shader);
	void sendShader();
	void activateShader();
	void setShaderUniform1f(const char* name, const float& kValue);
	void setShaderUniformMatrix4fv(const char* name, const glm::mat4& kMatrix);
	void setShaderUniform3f(const char* name, const glm::vec3& kMatrix);
	void setLightToShader(Light* p_light);
};

