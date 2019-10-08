#include "mainwindow.h"
#include "shader.h"
#include "vao.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

GLuint indices[] = {
	0,1,2,3,4,5,
	6,7,8,9,10,11,
	12,13,14,15,16,17,
	18,19,20,21,22,23,
	24,25,26,27,28,29,
	30,31,32,33,34,35
};

float planeVertices[] = {
	// positions          // texture Coords 
	 5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 2.0f, 2.0f
};

GLuint planeIndices[] = {
	0,1,2,
	3,4,5
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
	//camera
	Camera cam(glm::vec3(0, 0.f, 3.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));

	//object
	VAO cube;
	cube.create(vertices,sizeof(vertices)/sizeof(float),indices, sizeof(indices)/sizeof(GLuint));
	VAO plane;
	plane.create(planeVertices, sizeof(planeVertices) / sizeof(float), indices, sizeof(planeIndices) / sizeof(GLuint));
	
	ShaderProgram shader("bin/shader/obj_vert.glsl", "bin/shader/obj_frag.glsl");
	glm::mat4 model,view,proj;
	//model = glm::scale(model, glm::vec3(0.5f));
	model = glm::rotate(model, -20.f, glm::vec3(0, 1.f, 0));
	view = cam.getViewMat();
	proj = cam.getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);

	//light
	VAO light;
	light.create(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(GLuint));
	
	ShaderProgram lightShader("bin/shader/light_vert.glsl", "bin/shader/light_frag.glsl");
	glm::vec3 lightPos(1.5f, 1.5f, -2.f);
	glm::mat4 lightModel, lightView, lightProj;
	lightModel = glm::translate(lightModel, lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.2f));
	lightView = view;
	lightProj = proj;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.use();
		lightShader.setMat4f("M", lightModel);
		lightShader.setMat4f("V", lightView);
		lightShader.setMat4f("P", lightProj);
		light.draw();

		shader.use();
		shader.setMat4f("M", model);
		shader.setMat4f("V", view);
		shader.setMat4f("P", proj);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", cam.getPos());
		cube.draw();
		plane.draw();

		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();
}
