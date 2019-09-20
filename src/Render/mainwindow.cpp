#include "mainwindow.h"
#include "shader.h"
#include "vao.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
	//     ---- 位置 ----       ---- 法线 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

GLint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

MainWindow::MainWindow(int width,int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_pWindow = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
	
	if (m_pWindow == nullptr)
	{
		std::cout << "failed" << std::endl;
		glfwTerminate();
	}
	else {
		glfwMakeContextCurrent(m_pWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "faild" << std::endl;
		}
		else {
			//glfwSetFramebufferSizeCallback(m_pWindow, resizeWindow);
			glViewport(0, 0, width, height);
		}
	}

}

MainWindow::~MainWindow()
{
	delete m_pWindow;
}

void MainWindow::show()
{
	VAO square;
	square.create(vertices,32,indices,6);
	ShaderProgram shader("bin/shader/vert.glsl", "bin/shader/frag.glsl");
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(0.5));
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		shader.setMat4fv("M", trans);
		square.draw();
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();
}
