#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	// shader program ID
	unsigned int ID;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	// activate shader program
	void use();
	// uniform tools
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

