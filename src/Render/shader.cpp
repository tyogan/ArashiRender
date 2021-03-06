#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(const char* vertPath, const char* fragPath)
{
	//load file
	std::ifstream vertFile(vertPath);
	std::ifstream fragFile(fragPath);

	std::string vertCode((std::istreambuf_iterator<char>(vertFile)),
		(std::istreambuf_iterator<char>()));
	std::string fragCode((std::istreambuf_iterator<char>(fragFile)),
		(std::istreambuf_iterator<char>()));

	const char* strVertCode = vertCode.c_str();
	const char* strFragCode = fragCode.c_str();

	//build
	int success;
	char infoLog[512];
	GLuint vertId, fragId;
	vertId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertId, 1, &strVertCode, NULL);
	glCompileShader(vertId);
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &strFragCode, NULL);
	glCompileShader(fragId);
	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//link
	mID = glCreateProgram();
	glAttachShader(mID, vertId);
	glAttachShader(mID, fragId);
	glLinkProgram(mID);
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(mID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//delete
	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mID);
}

void ShaderProgram::use()
{
	glUseProgram(mID);
}

void ShaderProgram::release()
{
	glUseProgram(0);
}

void  ShaderProgram::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value);
}
void  ShaderProgram::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}
void  ShaderProgram::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}
void  ShaderProgram::setMat4f(const std::string &name, glm::mat4 m) const
{
	glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}
void  ShaderProgram::setVec3(const std::string &name, glm::vec3 v) const
{
	glUniform3fv(glGetUniformLocation(mID, name.c_str()), 1, glm::value_ptr(v));
}

void ShaderProgram::setBlock(const std::string &name, int idx)const
{
	glUniformBlockBinding(mID, glGetUniformBlockIndex(mID, name.c_str()), idx);
}