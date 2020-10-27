#include "hierarchywidget.h"

WidgetHierarchy::WidgetHierarchy(QWidget* parent, RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	this->setHeaderLabel("ObjectTree");
	loadObjectTree();
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