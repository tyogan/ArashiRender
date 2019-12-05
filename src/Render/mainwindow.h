#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

class RENDER_API MainWindow
{
public:
	MainWindow(int width, int height);
	~MainWindow();
	void show();

private:
	GLuint genTexture(std::string path);
	void genFrameTexture(GLuint& frameId,GLuint& id);
	void genShadowmap(GLuint& frameId, GLuint& id);

private:
	GLFWwindow* m_pWindow;
};