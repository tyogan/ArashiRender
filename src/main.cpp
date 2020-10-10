#include "Editor/mainwindow.h"

#include <QApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
	QApplication app(argc, argv);

	QOffscreenSurface* surface = new QOffscreenSurface;
	QOpenGLContext* context = new QOpenGLContext;

	surface->create();
	context->create();
	context->makeCurrent(surface);

	MainWindow win;
	win.setWindowTitle(QString("ArashiRender"));
	win.resize(QSize(960, 720));
	win.show();
	return app.exec();
}