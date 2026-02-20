#include "Shader.h"
#include <gtc/type_ptr.hpp>
#include "Path.h"
#include "Debug.h"
#include <fstream>
#include "Light.h"

Shader::Shader()
{
	m_vertex_id = glCreateShader(GL_VERTEX_SHADER);
	m_fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	m_program_id = glCreateProgram();
	debugLog("[Shader::Shader] The shader has created");
}

Shader::~Shader()
{
	if (m_program_id) glDeleteProgram(m_program_id);
}

void Shader::loadShader(const char* p_vertex_shader_name, const char* p_fragment_shader_name)
{
	if (!m_vertex_id) m_vertex_id = glCreateShader(GL_VERTEX_SHADER);
	if (!m_fragment_id) m_fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	if (!m_program_id) m_program_id = glCreateProgram();

	string l_vertex_folder_path = getAssetPath(Folder::Shaders, p_vertex_shader_name);
	if (l_vertex_folder_path.empty())
	{
		debugLog("[Shader::loadShader] invalid path to load a vertex shader", l_vertex_folder_path.c_str());
		return;
	}
	debugLog("[Shader::loadShader] The vertex path is valid");

	string l_fragment_folder_path = getAssetPath(Folder::Shaders, p_fragment_shader_name);
	if (l_fragment_folder_path.empty())
	{
		debugLog("[Shader::loadShader] invalid path to load a vertex shader", l_fragment_folder_path.c_str());
		return;
	}
	debugLog("[Shader::loadShader] The fragment path is valid");

	m_vertex_shader.clear();
	m_fragment_shader.clear();
	debugLog("[Shader::loadShader] The shader cointainers are clean");

	std::ifstream l_vertex_shader_file(l_vertex_folder_path.c_str(), std::ios::in);
	if (l_vertex_shader_file.is_open())
	{
		std::string line{};
		while (std::getline(l_vertex_shader_file, line))
		{
			m_vertex_shader += line + "\n";
		}
	}
	else
	{
		debugLog("[Shader::loadShader] couldn't open the vertex shader", l_vertex_folder_path.c_str());
		return;
	}
	l_vertex_shader_file.close();
	debugLog("[Shader::loadShader] The vertex shader is readed");

	std::ifstream l_fragment_shader_file(l_fragment_folder_path.c_str(), std::ios::in);
	if (l_fragment_shader_file.is_open())
	{
		std::string line{};
		while (std::getline(l_fragment_shader_file, line))
		{
			m_fragment_shader += line + "\n";
		}
	}
	else
	{
		debugLog("[Shader::loadShader] couldn't open the fragment shader", l_fragment_folder_path.c_str());
		return;
	}
	l_fragment_shader_file.close();
	debugLog("[Shader::loadShader] The fragment shader is readed");
}

void Shader::sendShader()
{
	const char* l_vertex_shader = m_vertex_shader.c_str();
	glShaderSource(m_vertex_id, 1, &l_vertex_shader, nullptr);
	glCompileShader(m_vertex_id);

	GLint success = 0;
	glGetShaderiv(m_vertex_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLength = 0;
		glGetShaderiv(m_vertex_id, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetShaderInfoLog(m_vertex_id, logLength, nullptr, log.data());

		debugLog("[Shader::sendShader]glCompileShader vertex shader error", log.c_str());
		glDeleteShader(m_vertex_id);
		m_vertex_id = 0;
		return;
	}
	else
	{
		debugLog("[Shader::sendShader] The vertex shader is compiled");
	}

	const char* l_fragment_shader = m_fragment_shader.c_str();
	glShaderSource(m_fragment_id, 1, &l_fragment_shader, nullptr);
	glCompileShader(m_fragment_id);

	glGetShaderiv(m_fragment_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLength = 0;
		glGetShaderiv(m_fragment_id, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetShaderInfoLog(m_fragment_id, logLength, nullptr, log.data());

		debugLog("[Shader::sendShader]glCompileShader fragment shader error", log.c_str());
		glDeleteShader(m_vertex_id);
		glDeleteShader(m_fragment_id);
		m_vertex_id = 0;
		m_fragment_id = 0;
		return;
	}
	else
	{
		debugLog("[Shader::sendShader] The fragment shader is compiled");
	}

	glAttachShader(m_program_id, m_vertex_id);
	glAttachShader(m_program_id, m_fragment_id);

	glLinkProgram(m_program_id);

	glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLength = 0;
		glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetProgramInfoLog(m_program_id, logLength, nullptr, log.data());

		debugLog("[Shader::sendShader]glLinkProgram link program error", log.c_str());

		glDeleteShader(m_vertex_id);
		glDeleteShader(m_fragment_id);
		glDeleteProgram(m_program_id);
		m_program_id = 0;
		m_vertex_id = 0;
		m_fragment_id = 0;
		return;
	}
	else
	{
		debugLog("[Shader::sendShader] The link program is complited");
	}

	glDeleteShader(m_vertex_id);
	glDeleteShader(m_fragment_id);
	m_vertex_id = 0;
	m_fragment_id = 0;

	model = glGetUniformLocation(m_program_id, "model");
	view = glGetUniformLocation(m_program_id, "view");
	projection = glGetUniformLocation(m_program_id, "projection");

	albedo = glGetUniformLocation(m_program_id, "albedo");
	normal_map = glGetUniformLocation(m_program_id, "normal_map");
	specular_map = glGetUniformLocation(m_program_id, "specular_map");
	environment_map = glGetUniformLocation(m_program_id, "environment_map");
	glossiness_metallic = glGetUniformLocation(m_program_id, "glossiness_metallic");
	time = glGetUniformLocation(m_program_id, "time");

	debugLog("[Shader::sendShader] All uniforms are set up");
}

void Shader::activateShader()
{
	glUseProgram(m_program_id);
}

void Shader::setShaderUniform1f(const char* name, const float& kValue)
{
	if (!m_program_id) return;
	GLint location = glGetUniformLocation(m_program_id, name);
	if (location != -1)
		glUniform1f(location, kValue);
}

void Shader::setShaderUniformMatrix4fv(const char* name, const glm::mat4& kMatrix)
{
	if (!m_program_id) return;
	GLint location = glGetUniformLocation(m_program_id, name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(kMatrix));
}

void Shader::setShaderUniform3f(const char* name, const glm::vec3& kVector)
{
	GLint location = glGetUniformLocation(m_program_id, name);
	if (location != -1)
		glUniform3f(location, kVector.x, kVector.y, kVector.z);
}

void Shader::setLightToShader(Light* p_light)
{
	activateShader();
	setShaderUniform3f("lightPos", p_light->getPosition());
	setShaderUniform3f("lightColor", p_light->getColor());
	setShaderUniform1f("lightIntensity", p_light->getIntensity());
	setShaderUniform1f("lightRadius", p_light->getRadius());
}
