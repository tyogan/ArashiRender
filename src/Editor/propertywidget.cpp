#include "propertywidget.h"

WidgetProperty::WidgetProperty(QWidget* parent,RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	this->setLayout(layout);

	QLabel* labelName=new QLabel("Name:");
	layout->addWidget(labelName);
	QLabel* labelScale = new QLabel("Scale:");
	layout->addWidget(labelScale);
	QLabel* labelTrans = new QLabel("Trans:");
	layout->addWidget(labelTrans);
	QLabel* labelRotate = new QLabel("Rotate:");
	layout->addWidget(labelRotate);
	QLabel* labelMatrial = new QLabel("Material:");
	layout->addWidget(labelMatrial);

}

void WidgetProperty::loadMeshProperty(int meshIdx)
{
	
}

