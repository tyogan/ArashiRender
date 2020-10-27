#pragma once

#include <QWidget>
#include <QTreeWidget>
#include "Render/renderscene.h"

class EDITOR_API WidgetHierarchy :public QTreeWidget
{
public:
	WidgetHierarchy(QWidget* parent, RenderScene* renderScene);

	void loadObjectTree();
private:
	RenderScene* mRenderScene;
};