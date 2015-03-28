#include <QApplication>

#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DMWindow* w = new DMWindow();
	
	w->new_danmaku("white", "white");

	return app.exec();
}

