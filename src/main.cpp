#include <QApplication>
#include <QDebug>
#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Subscriber subscriber("http://dm.tuna.moe/", "tuna", "", &app);
	DMWindow* w = new DMWindow();

	return app.exec();
}

