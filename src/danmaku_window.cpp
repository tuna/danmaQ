#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>

#ifdef	__linux
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#endif

#include <iostream>

#include "danmaku_ui.h"
#include "danmaku_window.h"

DMWindow::DMWindow() 
{
	QDesktopWidget desktop;
	QRect geo = desktop.screenGeometry();
	int sw = geo.width(), sh = geo.height();
	std::cout << sw << ", " << sh << std::endl;

	this->resize(sw, sh);
	this->setWindowTitle("Danmaku");
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowFlags(Qt::ToolTip|Qt::FramelessWindowHint);
	this->show();
	this->move(0, 0);

#ifdef __linux
	QRegion region;
	XShapeCombineRegion(
			QX11Info::display(), this->winId(),
			ShapeInput, 0, 0, region.handle(), ShapeSet);
#endif

}

void DMWindow::new_danmaku(QString text, QString color)
{
	Danmaku *l = new Danmaku(text, this);
	this->connect(l, SIGNAL(exited(Danmaku*)),
				  this, SLOT(delete_danmaku(Danmaku*)));
	l->show();
	l->move(200, 200);
}

void DMWindow::delete_danmaku(Danmaku* dm) {
	std::cout << "danmaku closed" << std::endl;
}
