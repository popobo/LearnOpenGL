#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
	void setFloats(const std::string& name, const std::vector<float>& values);
	void setMatrix4(const std::string& name, const glm::mat4& mat);
	void setVec3(const std::string& name, float x, float y, float z);
	void setVec3(const std::string& name, const glm::vec3& value) const;
};

