#include <QtCore>
#include <QtGui/qtextdocument.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QVector>
#include <QRegExp>
#include <QDebug>

#ifdef	__linux
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xregion.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#endif

#include <cstdlib>

#include "danmaku.h"


DMWindow::DMWindow(int screenNumber, DMApp *parent)
{
	this->setParent(parent);
	this->app = parent;
	QDesktopWidget desktop;
	QRect geo = desktop.screenGeometry(screenNumber);
	int sw = geo.width(), sh = geo.height();
	myDebug << sw << ", " << sh;
	this->resize(sw, sh);
	this->setWindowTitle("Danmaku");
	this->setWindowFlags(
		Qt::X11BypassWindowManagerHint
		| Qt::WindowStaysOnTopHint
		| Qt::ToolTip
		| Qt::FramelessWindowHint
	);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setAttribute(Qt::WA_Disabled, true);
	this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	this->setStyleSheet("background: transparent");

	this->move(geo.topLeft());
	this->init_slots();
	
	this->show();

#ifdef __linux
	Region region = XCreateRegion();
	XShapeCombineRegion(
			QX11Info::display(), this->winId(),
			ShapeInput, 0, 0, region, ShapeSet);
#endif

}

DMWindow::DMWindow(DMApp *parent): DMWindow(0, parent){};

void DMWindow::init_slots()
{
	int height = this->height();
	int nlines = (height - 2*VMARGIN) / (this->app->lineHeight);
	myDebug << nlines << this->app->lineHeight;
	for(int i=0; i<nlines; i++) {
		this->fly_slots.append(false);
		this->fixed_slots.append(false);
	}
}

int DMWindow::allocate_slot(Position position) {
// 	if(position == "fly")
//
	int slot = -1;
	switch (position) {
	case FLY:
		for (int i=0; i < 6; i++) {
			int try_slot;
			if (i < 3) {
				try_slot = std::rand() % (this->fly_slots.size() / 2);
			} else {
				try_slot = std::rand() % (this->fly_slots.size());
			}
			if(this->fly_slots.at(try_slot) == false) {
				this->fly_slots[try_slot] = true;
				slot = try_slot;
				break;
			}
		}
		break;
	case TOP:
		for(int i=0; i < this->fixed_slots.size(); i++) {
			if(this->fixed_slots.at(i) == false) {
				this->fixed_slots[i] = true;
				slot = i;
				break;
			}
		}
		break;
	case BOTTOM:
		for(int i=this->fixed_slots.size()-1; i >= 0; i--) {
			if(this->fixed_slots.at(i) == false) {
				this->fixed_slots[i] = true;
				slot = i;
				break;
			}
		}
		break;
	}
	myDebug << "Slot: " << slot;
	return slot;
}

int DMWindow::slot_y(int slot)
{
	return (this->app->lineHeight * slot + VMARGIN);
}

QString DMWindow::escape_text(QString & text) {
	QString escaped = text.toHtmlEscaped();

	escaped.replace(QRegExp("([^\\\\])\\\\n"), "\\1<br/>");
	escaped.replace(QRegExp("\\\\\\\\n"), "\\n");
	escaped.replace(QRegExp("\\[s\\](.+)\\[/s\\]"), "<s>\\1</s>");

	return escaped;
}

void DMWindow::new_danmaku(QString text, QString color, QString position)
{
	Position pos;
	if(position.compare("fly") == 0) {
		myDebug << "fly";
		pos = FLY;
	} else if (position.compare("top") == 0) {
		myDebug << "top";
		pos = TOP;
	} else if (position.compare("bottom") == 0) {
		myDebug << "bottom";
		pos = BOTTOM;
	} else {
		myDebug << "wrong position: " << position;
		return;
	}

	auto slot = allocate_slot(pos);
	if (slot < 0) {
		myDebug << "Screen is Full!";
		return;
	} 

	Danmaku *l = new Danmaku(escape_text(text), color, pos, slot, this, this->app);
	this->connect(l, SIGNAL(exited(Danmaku*)),
				  this, SLOT(delete_danmaku(Danmaku*)));
	this->connect(l, SIGNAL(clear_fly_slot(int)),
				this, SLOT(clear_fly_slot(int)));
	l->show();
	// l->move(200, 200);
}

void DMWindow::clear_fly_slot(int slot) {
	myDebug << "Clear Flying Slot: " << slot;
	// myDebug << this->fly_slots;
	this->fly_slots[slot] = false;
}

void DMWindow::delete_danmaku(Danmaku* dm) {
	if (dm->position == TOP || dm->position == BOTTOM) {
		this->fixed_slots[dm->slot] = false;
	}
	myDebug << "danmaku closed";
}

DMWindow::~DMWindow() {
	myDebug << "window closed";
}
