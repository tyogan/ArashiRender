#include <iostream>
#include "Render/mainwindow.h"

int main()
{
	MainWindow window(800, 600);
	if (window.init() != -1)
	{
		window.showImage();
	}
	return 0;
}