#pragma once

#include <memory>

#include <QWidget>

#include "Render/renderscene.h"

using std::shared_ptr;

class EDITOR_API WidgetProperty :public QWidget
{
public:
	WidgetProperty(QWidget* parent, RenderScene* renderScene);

public slots:
	void loadMeshProperty(int meshIdx);

private:
	RenderScene* mRenderScene;

};