#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QFile>

#include "Render/renderscene.h"

class EDITOR_API WidgetSetting :public QWidget
{
	Q_OBJECT
public:
	WidgetSetting(QWidget* parent, RenderScene* renderScene);

signals:
	void resetEnvmapLight();

public slots:
	void setMaterial(int mat);
	void reloadEnvmap(bool isClicked);

private:
	RenderScene* mRenderScene;

};