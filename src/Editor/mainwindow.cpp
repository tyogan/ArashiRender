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

	QMenuBar* menuBar = new QMenuBar(this);
	QMenu* objMenu = menuBar->addMenu(tr("&Object"));

	QAction* actionLoadObj = objMenu->addAction("load");
	QMenu* menuCreateObj = objMenu->addMenu("create");

	QAction* actioncreatePlane = menuCreateObj->addAction("Plane");
	QAction* actioncreateCube = menuCreateObj->addAction("Cube");
	QAction* actioncreateSphere = menuCreateObj->addAction("Sphere");
	objMenu->addSeparator();

	menuBar->addAction(tr("&Envmap"));
	objMenu->addSeparator();
	QMenu* HHH = menuBar->addMenu(tr("&HHHH"));
}