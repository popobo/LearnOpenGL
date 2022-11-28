#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace {
	const int INFO_BUFFER_SIZE = 512 ;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCodeStr;
	std::string fragmentCodeStr;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCodeStr = vShaderStream.str();
		fragmentCodeStr = fShaderStream.str();
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << e.what() << std::endl;
	}

	const char* vShaderCodeCStr = vertexCodeStr.c_str();
	const char* fShaderCodeCStr = fragmentCodeStr.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[INFO_BUFFER_SIZE];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCodeCStr, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, INFO_BUFFER_SIZE, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCodeCStr, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, INFO_BUFFER_SIZE, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloats(const std::string& name, const std::vector<float>& values)
{	
	if (values.size() > 4 || values.size() < 1) {
		return;
	}
	int location = glGetUniformLocation(ID, name.c_str());
	switch (values.size())
	{
	case 1:
		glUniform1f(location, values[0]);
		break;
	case 2:
		glUniform2f(location, values[0], values[1]);
		break;
	case 3:
		glUniform3f(location, values[0], values[1], values[2]);
		break;
	case 4:
		glUniform4f(location, values[0], values[1], values[2], values[3]);
		break;
	default:
		break;
	}
}

void Shader::setMatrix4(const std::string& name, const glm::mat4& mat)
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::setVec3(const std::string& name, float x, float y, float z) {
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform3f(location, x, y, z);
}