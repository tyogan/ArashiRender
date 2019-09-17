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
	GLFWwindow* m_pWindow;
};