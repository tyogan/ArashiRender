#include <iostream>

class RENDER_API Shader
{
public:
	Shader(int num);
	~Shader();

	void print();

private:
	int m_nNum;
};