#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QFileDialog>
#include <QFile>
#include <QAction>

#include "Render/renderscene.h"

class EDITOR_API WidgetHierarchy :public QTreeWidget
{
public:
	WidgetHierarchy(QWidget* parent, RenderScene* renderScene);

	void loadObjectTree();
private:

	void initObjectActions();
	void openObjectFile();
	void createPlane();
	void createCube();
	void createSphere();

private:
	RenderScene* mRenderScene;
};