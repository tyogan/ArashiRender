#include "hierarchywidget.h"

WidgetHierarchy::WidgetHierarchy(QWidget* parent, RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	this->setHeaderLabel("ObjectTree");
	this->setContextMenuPolicy(Qt::ActionsContextMenu);
	loadObjectTree();
	initObjectActions();
}

void WidgetHierarchy::loadObjectTree()
{
	for (int i = 0; i < mRenderScene->mRenderMeshParam.size(); i++)
	{
		QString meshName = mRenderScene->mRenderMeshParam[i].mObjectName.c_str();
		QTreeWidgetItem* meshItem = new QTreeWidgetItem(this, QStringList(meshName));
		this->insertTopLevelItem(i, meshItem);
	}
}

void WidgetHierarchy::initObjectActions()
{
	QAction* actioncreatePlane = new QAction("Create Plane");
	QAction* actioncreateCube = new QAction("Create Cube");
	QAction* actioncreateSphere = new QAction("Create Sphere");
	QAction* actionLoadObject = new QAction("Load Object");

	addAction(actioncreatePlane);
	addAction(actioncreateCube);
	addAction(actioncreateSphere);
	addAction(actionLoadObject);

	connect(actionLoadObject, &QAction::triggered, this, &WidgetHierarchy::openObjectFile);
	connect(actioncreatePlane, &QAction::triggered, this, &WidgetHierarchy::createPlane);
	connect(actioncreateCube, &QAction::triggered, this, &WidgetHierarchy::createCube);
	connect(actioncreateSphere, &QAction::triggered, this, &WidgetHierarchy::createSphere);
}

void WidgetHierarchy::openObjectFile()
{
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "*.obj");

	if (fileName == "")
	{
		return;
	}
	else {
		mRenderScene->addSceneMesh(fileName.toStdString(), glm::mat4(1.f), glm::mat4(1.f));
	}
}

void WidgetHierarchy::createPlane()
{
	mRenderScene->addSceneMesh(ModelType::PLANE, glm::mat4(1.f), glm::mat4(1.f));
}

void WidgetHierarchy::createCube()
{
	mRenderScene->addSceneMesh(ModelType::CUBE, glm::mat4(1.f), glm::mat4(1.f));
}

void WidgetHierarchy::createSphere()
{
	mRenderScene->addSceneMesh(ModelType::SPHERE, glm::mat4(1.f), glm::mat4(1.f));
}