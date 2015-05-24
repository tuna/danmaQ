#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <ctime>

#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	std::srand(std::time(0));

	QDesktopWidget desktop;
	DMApp dm_app(argv[1], argv[2], "");
	app.connect(
		&desktop, SIGNAL(workAreaResized(int)),
		&dm_app, SLOT(reset_windows())
	);
	return app.exec();
}

