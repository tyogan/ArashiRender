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
	delete mPropertyWidget;
	delete mHierarchyWidget;
	delete mRenderScene;
}

void MainWindow::initUI()
{
	mRenderWidget = new WidgetRender(this);
	this->setCentralWidget(mRenderWidget);
	mRenderWidget->setRenderScene(mRenderScene);

	mPropertyWidget = new WidgetProperty(this, mRenderScene);
	QDockWidget* propertyDock = new QDockWidget(this);
	propertyDock->setWidget(mPropertyWidget);
	propertyDock->setWindowTitle("Property");
	this->addDockWidget(Qt::LeftDockWidgetArea, propertyDock);

	mHierarchyWidget = new WidgetHierarchy(this,mRenderScene);
	QDockWidget* hierarchyDock = new QDockWidget(this);
	hierarchyDock->setWidget(mHierarchyWidget);
	hierarchyDock->setWindowTitle("Hierarchy");
	hierarchyDock->setObjectName("HierarchyDock");
	this->addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

	QDockWidget* settingDock = new QDockWidget(this);
	settingDock->setWindowTitle("Setting");
	this->addDockWidget(Qt::LeftDockWidgetArea, settingDock);

	this->tabifyDockWidget(hierarchyDock, settingDock);
	hierarchyDock->raise();

	initMenuBar();
	initToolBar();
	initConnection();
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

	//render type
	QMenu* menuPipline = pMenuBar->addMenu("Pipline");
	QAction* actionforward = menuPipline->addAction("Forward");
	QAction* actionDeferred = menuPipline->addAction("Deferred");
	menuPipline->addSeparator();
	QAction* actionOffline = menuPipline->addAction("OffLine");

	//load mesh
	QMenu* menuScene = pMenuBar->addMenu(tr("&SceneObject"));
	QMenu* menuObject = menuScene->addMenu("Geometry");
	QAction* actionLoadObject = menuObject->addAction("Load Object");
	QMenu* menuCreateObject = menuObject->addMenu("Create Object");
	QAction* actioncreatePlane = menuCreateObject->addAction("Plane");
	QAction* actioncreateCube = menuCreateObject->addAction("Cube");
	QAction* actioncreateSphere = menuCreateObject->addAction("Sphere");
	//mesh connection
	connect(actionLoadObject, &QAction::triggered, this, &MainWindow::openObjectFile);
	connect(actioncreatePlane, &QAction::triggered, this, &MainWindow::createPlane);
	connect(actioncreateCube, &QAction::triggered, this, &MainWindow::createCube);
	connect(actioncreateSphere, &QAction::triggered, this, &MainWindow::createSphere);

	//envmap
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

void MainWindow::initConnection()
{
	connect(mRenderWidget, &WidgetRender::pickMesh, mPropertyWidget, &WidgetProperty::loadMeshProperty);
}

void MainWindow::openObjectFile()
{
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, "Open File", QString(),"*.obj");

	if (fileName == "")
	{
		return;
	}
	else {
		mRenderScene->addSceneMesh(fileName.toStdString(), glm::mat4(1.f), glm::mat4(1.f));
	}
}

void MainWindow::createPlane()
{
	mRenderScene->addSceneMesh(ModelType::PLANE, glm::mat4(1.f), glm::mat4(1.f));
}

void MainWindow::createCube()
{
	mRenderScene->addSceneMesh(ModelType::CUBE, glm::mat4(1.f), glm::mat4(1.f));
}

void MainWindow::createSphere()
{
	mRenderScene->addSceneMesh(ModelType::SPHERE, glm::mat4(1.f), glm::mat4(1.f));
}