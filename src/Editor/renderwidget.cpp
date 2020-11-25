#include "renderwidget.h"

#include <QDebug>
#include <QtGui>
#include <QtWidgets>

WidgetRender::WidgetRender(QWidget* parent)
	:QWidget(parent),mMeshIdx(-1)
{
	mFramebuffer = new FrameBuffer(960, 720);
	this->setFixedSize(960, 720);
}

WidgetRender::~WidgetRender()
{
	delete mRenderScene;
	delete mFramebuffer;
	delete mRender;
}

void WidgetRender::setRenderScene(RenderScene* renderScene)
{
	mRenderScene = renderScene;
	mRender = new GLRender();
	mRender->init();
}

void WidgetRender::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	
	mRender->render(mFramebuffer, mRenderScene);
	QImage image(mFramebuffer->mWidth, mFramebuffer->mHeight, QImage::Format_RGB888);
	mFramebuffer->dump(image.bits());
	painter.drawImage(0, 0, image.mirrored());
}

void WidgetRender::mouseMoveEvent(QMouseEvent * event)
{
	event->accept();
	if (!mRenderScene)
		return;
	if (mMeshIdx == -1) 
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
		mRenderScene->mRenderMeshParam[mMeshIdx].mTrans = m * mRenderScene->mRenderMeshParam[mMeshIdx].mTrans;
		break;
	}
	case 1:
	{
		dx = -dx / width()*180.f;
		dy = -dy / height()*180.f;
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, dy, glm::vec3(1, 0, 0));
		rotate = glm::rotate(rotate, dx, glm::vec3(0, 1, 0));

		mRenderScene->mRenderMeshParam[mMeshIdx].mRotate = rotate * mRenderScene->mRenderMeshParam[mMeshIdx].mRotate;

		break;
	}
	default:
		break;
	}

	update();

	mMousePos = event->pos();
}

void WidgetRender::mousePressEvent(QMouseEvent * event)
{
	mMousePos = event->pos();
	int hhh = height();
	glm::vec3 mIdx = mFramebuffer->getMIdxPixel(mMousePos.x(), height() - mMousePos.y() - 1);

	mMeshIdx = mIdx.x;
	emit pickMesh(mMeshIdx);
}