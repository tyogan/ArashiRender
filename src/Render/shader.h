#pragma once
#include <iostream>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class RENDER_API ShaderProgram
{
public:
	ShaderProgram(const char* vertPath,const char* fragPath);
	~ShaderProgram();

	void use();
	void release();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4f(const std::string &name, glm::mat4 m)const;
	void setVec3(const std::string &name, glm::vec3 v)const;
	void setBlock(const std::string &name, int idx)const;
private:
	GLuint m_ID;
};