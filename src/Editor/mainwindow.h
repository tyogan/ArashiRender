#pragma once
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QStyle>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QFile>

#include "renderwidget.h"
#include "hierarchywidget.h"
#include "settingwidget.h"

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
	void initToolBar();
	void initConnection();

private:
	WidgetRender* mRenderWidget;
	WidgetHierarchy* mHierarchyWidget;
	WidgetSetting* mSettingWidget;

	RenderScene* mRenderScene;
};