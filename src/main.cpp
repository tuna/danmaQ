#include <QApplication>
#include <QDebug>
#include "danmaku.h"
#include <ctime>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	// Subscriber subscriber("http://dm.tuna.moe/", "tuna", "");
	Subscriber subscriber(argv[1], argv[2], "");
	DMWindow* w = new DMWindow();
	std::srand(std::time(0));
	app.connect(
		&subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
		w, SLOT(new_danmaku(QString, QString, QString))
	);
	subscriber.start();
	return app.exec();
}

