#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <ctime>

#include "danmaQ_app.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	std::srand(std::time(0));

	QDesktopWidget desktop;
	DMApp* dm_app = new DMApp();
	app.connect(
		&desktop, SIGNAL(workAreaResized(int)),
		dm_app, SLOT(reset_windows())
	);
	return app.exec();
}

