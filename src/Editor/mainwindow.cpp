#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent)
{
	mRenderScene = new RenderScene();
	mRenderScene->init();

	initUI();
}

MainWindow::~MainWindow()
{
	delete mRenderWidget;
}

void MainWindow::initUI()
{
	mRenderWidget = new RenderWidget(this);
	this->setCentralWidget(mRenderWidget);
	mRenderWidget->setRenderScene(mRenderScene);
}