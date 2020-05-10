#include "mainwindow.h"

MainWindow::MainWindow(int width, int height)
	:mWidth(width), mHeight(height)
{
}

MainWindow::~MainWindow()
{
	delete mWindow;
}

int MainWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "Render", nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glfwSetFramebufferSizeCallback(mWindow, resizeWindow);
	glViewport(0, 0, mWidth, mHeight);
	return 0;
}

void MainWindow::showImage()
{	
	ShaderProgram imageShader("bin/shader/image_vert.glsl", "bin/shader/image_frag.glsl");
	Object* imageObj = new Object();
	imageObj->createPlane();

	GLRender ren;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(mWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ren.render();
		imageShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ren.getTexture());
		imageObj->draw();
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}
	glfwTerminate();
}
