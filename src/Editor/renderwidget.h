#pragma once

#include <QWidget>
#include <QPainter>

#include "Render/render.h"


class EDITOR_API RenderWidget :public QWidget
{
public:
	RenderWidget(QWidget* parent);
	~RenderWidget();

	void setRenderScene(RenderScene* renderScene);

	virtual void paintEvent(QPaintEvent* event);

	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;

private:
	FrameBuffer* mFramebuffer;
	RenderScene* mRenderScene;

	GLRender* mRender;
	QPoint mMousePos;
	int mMeshIdx;
};