#include <QApplication>
#include <QDebug>
#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Subscriber subscriber("http://localhost:5000/", "tuna", "");
	DMWindow* w = new DMWindow();
	app.connect(
		&subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
		w, SLOT(new_danmaku(QString, QString, QString))
	);
	subscriber.start();
	return app.exec();
}

