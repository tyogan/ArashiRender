#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "vao.h"
#include "camera.h"
#include "shadowmap.h"
#include "mesh.h"
#include "importer.h"
#include "render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

class RENDER_API MainWindow
{
public:
	MainWindow(int width, int height);
	~MainWindow();
	int init();
	void showImage();
private:
	int mWidth, mHeight;
	GLFWwindow* mWindow;
};
