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
}

void MainWindow::initUI()
{
	mRenderWidget = new WidgetRender(this);
	this->setCentralWidget(mRenderWidget);
	mRenderWidget->setRenderScene(mRenderScene);
	
	mHierarchyWidget = new WidgetHierarchy();
	QDockWidget* hierarchyDock = new QDockWidget(this);
	hierarchyDock->setWidget(mHierarchyWidget);
	hierarchyDock->setWindowTitle("Hierarchy");
	hierarchyDock->setObjectName("HierarchyDock");
	this->addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

	QDockWidget* settingDock = new QDockWidget(this);
	settingDock->setWindowTitle("Setting");
	this->addDockWidget(Qt::LeftDockWidgetArea, settingDock);

	QDockWidget* propertyDock = new QDockWidget(this);
	propertyDock->setWindowTitle("Property");
	this->addDockWidget(Qt::RightDockWidgetArea, propertyDock);

	initMenuBar();
	initToolBar();
}

void MainWindow::initMenuBar()
{
	QMenuBar* pMenuBar = menuBar();

	QMenu* menuFile = pMenuBar->addMenu(tr("&File"));
	QAction* actionNewScene = menuFile->addAction("New Scene");
	QAction* actionOpenScene = menuFile->addAction("Open Scene");
	menuFile->addSeparator();
	QAction* actionSave = menuFile->addAction("Save");
	QAction* actionSaveAs = menuFile->addAction("Save As...");
	menuFile->addSeparator();
	QAction* actionSaveImage = menuFile->addAction("Save Image");
	menuFile->addSeparator();
	QAction* actionExit = menuFile->addAction("Exit");

	QMenu* menuPipline = pMenuBar->addMenu("Pipline");
	QAction* actionforward = menuPipline->addAction("Forward");
	QAction* actionDeferred = menuPipline->addAction("Deferred");
	menuPipline->addSeparator();
	QAction* actionOffline = menuPipline->addAction("OffLine");

	QMenu* menuScene = pMenuBar->addMenu(tr("&SceneObject"));
	QMenu* menuObject = menuScene->addMenu("Geometry");
	QAction* actionLoadObject = menuObject->addAction("Load Object");
	QMenu* menuCreateObject = menuObject->addMenu("Create Object");
	QAction* actioncreatePlane = menuCreateObject->addAction("Plane");
	QAction* actioncreateCube = menuCreateObject->addAction("Cube");
	QAction* actioncreateSphere = menuCreateObject->addAction("Sphere");
	QAction* actionEnvmap = menuScene->addAction(tr("&Envmap"));
	QAction* actionLight = menuScene->addAction(tr("&Light"));
}

void MainWindow::initToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	this->addToolBar(toolBar);
	QAction* actionRotateCamera = toolBar->addAction("Camera");
	QAction* actionRotateObject = toolBar->addAction("Object");
	toolBar->addSeparator();
	QAction* actionMove = toolBar->addAction("Move");
	QAction* actionRotate = toolBar->addAction("Rotate");
	toolBar->addSeparator();
}