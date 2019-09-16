#include "shader.h"

Shader::Shader(int num)
{
	m_nNum = num;
}

Shader::~Shader()
{
}

void Shader::print()
{
	std::cout << m_nNum << std::endl;
}