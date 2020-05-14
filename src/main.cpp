#include <iostream>
#include "Render/mainwindow.h"

int main()
{
	MainWindow window(960, 720);
	if (window.init() != -1)
	{
		window.showImage();
	}
	return 0;
}