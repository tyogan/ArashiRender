#pragma once

#include <QWidget>
#include <QPainter>

#include "Render/render.h"


class EDITOR_API WidgetRender :public QWidget
{
	Q_OBJECT
public:
	WidgetRender(QWidget* parent);
	~WidgetRender();

	void setRenderScene(RenderScene* renderScene);
	virtual void paintEvent(QPaintEvent* event);

	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;

signals:
	void pickMesh(int meshIdx);

public slots:
	void resetEnvmapLight();

private:
	FrameBuffer* mFramebuffer;
	RenderScene* mRenderScene;

	GLRender* mRender;
	QPoint mMousePos;
	int mMeshIdx;
};