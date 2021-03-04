#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent)
{
	this->setMinimumWidth(1280);
	mRenderScene = new RenderScene();
	mRenderScene->init();

	initUI();
}

MainWindow::~MainWindow()
{
	delete mRenderWidget;
	delete mHierarchyWidget;
	delete mRenderScene;
}

void MainWindow::initUI()
{
	mRenderWidget = new WidgetRender(this);
	this->setCentralWidget(mRenderWidget);
	mRenderWidget->setRenderScene(mRenderScene);

	mHierarchyWidget = new WidgetHierarchy(this,mRenderScene);
	QDockWidget* hierarchyDock = new QDockWidget(this);
	hierarchyDock->setWidget(mHierarchyWidget);
	hierarchyDock->setWindowTitle("Hierarchy");
	hierarchyDock->setObjectName("HierarchyDock");
	this->addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

	mSettingWidget = new WidgetSetting(this, mRenderScene);
	QDockWidget* settingDock = new QDockWidget(this);
	settingDock->setWidget(mSettingWidget);
	settingDock->setWindowTitle("Setting");
	this->addDockWidget(Qt::LeftDockWidgetArea, settingDock);

	initToolBar();
	initConnection();
}

void MainWindow::initToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	this->addToolBar(toolBar);
	QAction* actionRotateSave= toolBar->addAction("Save");
	toolBar->addSeparator();
	QAction* actionRotateCamera = toolBar->addAction("Camera");
	QAction* actionLight = toolBar->addAction("Light");
	toolBar->addSeparator();
}

void MainWindow::initConnection()
{
	connect(mSettingWidget, &WidgetSetting::resetEnvmapLight, mRenderWidget, &WidgetRender::resetEnvmapLight);
	update();
}