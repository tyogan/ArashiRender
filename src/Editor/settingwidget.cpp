#include "settingwidget.h"

WidgetSetting::WidgetSetting(QWidget* parent, RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	this->setLayout(layout);

	QLabel* labelMatrial = new QLabel("Material:");
	QComboBox* comboboxMaterial = new QComboBox();
	comboboxMaterial->addItem("Phong");
	comboboxMaterial->addItem("SSAO");
	comboboxMaterial->addItem("SH");
	comboboxMaterial->addItem("IBL");
	QHBoxLayout* layoutMaterial = new QHBoxLayout();
	layoutMaterial->addWidget(labelMatrial);
	layoutMaterial->addWidget(comboboxMaterial);
	layout->addLayout(layoutMaterial);


	QLabel* labelPipline = new QLabel("Pipline:");
	QComboBox* comboboxPipline = new QComboBox();
	comboboxPipline->addItem("Forward");
	comboboxPipline->addItem("Deffered");
	comboboxPipline->addItem("Offline");
	QHBoxLayout* layoutPipline = new QHBoxLayout();
	layoutPipline->addWidget(labelPipline);
	layoutPipline->addWidget(comboboxPipline);
	layout->addLayout(layoutPipline);

	QHBoxLayout* layoutEnvmap = new QHBoxLayout();
	QLabel* labelEnvmap = new QLabel("Envmap:");

	QPushButton* buttonHdr = new QPushButton("load");
	layoutEnvmap->addWidget(labelEnvmap);
	layoutEnvmap->addWidget(buttonHdr);
	layout->addLayout(layoutEnvmap);

	//connect
	connect(comboboxMaterial, qOverload<int>(&QComboBox::currentIndexChanged), this, &WidgetSetting::setMaterial);
	connect(buttonHdr, &QPushButton::clicked, this, &WidgetSetting::reloadEnvmap);

}

void WidgetSetting::setMaterial(int mat)
{
	for (int i = 0; i < mRenderScene->mRenderMeshParam.size(); i++)
	{
		mRenderScene->mRenderMeshParam[i].mMatIdx = mat;
	}
}

void WidgetSetting::reloadEnvmap(bool isClicked)
{
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "*.hdr");
	mRenderScene->mEnvmap->load(fileName.toStdString());
	emit resetEnvmapLight();
}