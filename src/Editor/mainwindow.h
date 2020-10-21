#pragma once
#include <QMainWindow>
#include <QToolBar>
#include <QStyle>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "wrender.h"
#include "wthierarchy.h"
#include "wsetting.h"
#include "wproperty.h"

#include "Render/renderscene.h"

#include <memory>
using std::shared_ptr;

class EDITOR_API MainWindow:public QMainWindow
{
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void initUI();
	void initMenuBar();
	void initToolBar();
private:
	WidgetRender* mRenderWidget;
	WidgetHierarchy* mHierarchyWidget;
	WidgetSetting* mSettingWidget;
	WidgetProperty* mPropertyWidget;

	RenderScene* mRenderScene;
};