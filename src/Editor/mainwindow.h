#pragma once
#include <QMainWindow>
#include <QToolBar>
#include <QStyle>
#include <QMenu>
#include <QMenuBar>

#include "renderwidget.h"
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

private:
	RenderWidget* mRenderWidget;
	RenderScene* mRenderScene;
};