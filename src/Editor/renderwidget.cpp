#include "renderwidget.h"

#include <QDebug>
#include <QtGui>
#include <QtWidgets>

RenderWidget::RenderWidget(QWidget* parent)
	:QWidget(parent)
{
	mFramebuffer = new FrameBuffer(960, 720);
}

RenderWidget::~RenderWidget()
{
	delete mRenderScene;
	delete mFramebuffer;
	delete mRender;
}

void RenderWidget::setRenderScene(RenderScene* renderScene)
{
	mRenderScene = renderScene;
	mRender = new GLRender(renderScene);
}

void RenderWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	
	mRender->render(mFramebuffer);
	QImage image(mFramebuffer->mWidth, mFramebuffer->mHeight, QImage::Format_RGB888);
	mFramebuffer->dump(image.bits());
	painter.drawImage(0, 0, image.mirrored());
}

void RenderWidget::mouseMoveEvent(QMouseEvent * event)
{
	event->accept();
	if (!mRenderScene)
		return;

	int mode = 0;
	if (event->buttons() == Qt::RightButton)
	{
		mode = 1;
	}

	QPoint pos = event->pos();
	float dx = float(mMousePos.x() - pos.x());
	float dy = float(mMousePos.y() - pos.y());

	glm::mat4 V = mRenderScene->mScene->mCamera->getViewMat();
	glm::mat4 P = mRenderScene->mScene->mCamera->getProjMat();

	glm::vec4 posTrans = glm::vec4(1, 1, 0, 1);
	posTrans = P * V * posTrans;

	float dx0 = (posTrans.x / posTrans.w / 2.f)*width();
	float dy0 = (posTrans.y / posTrans.w / 2.f)*height();

	switch (mode)
	{
	case 0:
	{
		glm::mat4 m;
		m = glm::translate(m, glm::vec3(-dx / dx0, dy / dy0, 0));
		mRenderScene->mSceneMeshParam[0].mTrans = m * mRenderScene->mSceneMeshParam[0].mTrans;
		break;
	}
	case 1:
	{
		dx = -dx / width()*180.f;
		dy = -dy / height()*180.f;
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, dy, glm::vec3(1, 0, 0));
		rotate = glm::rotate(rotate, dx, glm::vec3(0, 1, 0));

		mRenderScene->mSceneMeshParam[0].mRotate = rotate * mRenderScene->mSceneMeshParam[0].mRotate;

		break;
	}
	default:
		break;
	}

	update();

	mMousePos = event->pos();
}

void RenderWidget::mousePressEvent(QMouseEvent * event)
{
	mMousePos = event->pos();
}