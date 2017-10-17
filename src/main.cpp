#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <ctime>
#include <cstdlib>

#include "danmaQ_app.h"

#if defined _WIN32 || defined __CYGWIN__
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    std::srand(std::time(0));

	QDesktopWidget* desktop = QApplication::desktop();
	DMApp* dm_app = new DMApp();
	app.connect(
		desktop, SIGNAL(workAreaResized(int)),
		dm_app, SLOT(reset_windows())
	);
	app.connect(
		desktop, SIGNAL(screenCountChanged(int)),
		dm_app, SLOT(reset_windows())
	);
	return app.exec();
}

