#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <ctime>

#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	// Subscriber subscriber("http://dm.tuna.moe/", "tuna", "");
	Subscriber subscriber(argv[1], argv[2], "");
	std::srand(std::time(0));

	QDesktopWidget desktop;
	DMApp dm_app(&subscriber);
	app.connect(
		&desktop, SIGNAL(workAreaResize(int)),
		&dm_app, SLOT(reset_windows())
	);
	// dm_app.reset_windows();
	// QDesktopWidget desktop;
	// for (int i=0; i<desktop.screenCount(); i++) {
	// 	DMWindow* w = new DMWindow(i);
	// 	app.connect(
	// 		&subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
	// 		w, SLOT(new_danmaku(QString, QString, QString))
	// 	);
	// }
	subscriber.start();
	return app.exec();
}

