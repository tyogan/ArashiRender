#include "mainwindow.h"
#include "shader.h"
#include "vao.h"
#include "camera.h"
#include "vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

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
	Camera cam(glm::vec3(0, 0, 10.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
	GLuint testTexture = genTexture();
	GLuint frameId,textureId;
	genFrameTexture(frameId,textureId);
	GLuint shadowFrameId, shadowTextureId;
	genFrameTexture(shadowFrameId, shadowTextureId);

	//object
	ShaderProgram objShader("bin/shader/obj_vert.glsl", "bin/shader/obj_frag.glsl");
	
	VAO cube;
	cube.create(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(GLuint));
	VAO plane;
	plane.create(planeVertices, sizeof(planeVertices) / sizeof(float), indices, sizeof(planeIndices) / sizeof(GLuint));
	glm::mat4 model, view, proj;
	model = glm::translate(model, glm::vec3(0, -2.f, 0));
	//model = glm::scale(model, glm::vec3(0.5f));
	model = glm::rotate(model, -15.f, glm::vec3(0, 1.f, 0));
	view = cam.getViewMat();
	proj = cam.getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);

	//shadowShader
	ShaderProgram shadowShader("bin/shader/light_vert.glsl", "bin/shader/light_frag.glsl");
	//shadowShader
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 shadowProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(glm::vec3(2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	//image
	ShaderProgram imageShader("bin/shader/image_vert.glsl", "bin/shader/image_frag.glsl");
	VAO imageVert;
	imageVert.create(imageVertices, sizeof(imageVertices) / sizeof(float), imageIndices, sizeof(imageIndices) / sizeof(GLuint));

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, frameId);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		objShader.use();
		objShader.setMat4f("M", model);
		objShader.setMat4f("V", view);
		objShader.setMat4f("P", proj);
		objShader.setVec3("lightDir", glm::vec3(2.0f, 4.0f, 1.0f));
		objShader.setVec3("viewPos", cam.getPos());
		objShader.setInt("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, testTexture);
		cube.draw();
		plane.draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameId);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader.use();
		shadowShader.setMat4f("M", model);
		shadowShader.setMat4f("V", shadowProjection);
		shadowShader.setMat4f("P", shadowView);
		cube.draw();
		plane.draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		imageShader.use();
		glBindTexture(GL_TEXTURE_2D, shadowTextureId);
		imageVert.draw();

		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();
}

GLuint MainWindow::genTexture()
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("bin/shader/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return id;
}

void MainWindow::genFrameTexture(GLuint& frameId,GLuint& id)
{
	glGenFramebuffers(1, &frameId);
	glGenTextures(1, &id);

	glBindFramebuffer(GL_FRAMEBUFFER, frameId);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	std::cout << "framebuffer";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainWindow::genShadowmap(GLuint& frameId, GLuint& id)
{
	glGenFramebuffers(1, &frameId);
	glGenTextures(1, &id);
	
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}