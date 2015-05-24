#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>
#include <QVector>
#include <QDebug>

#ifdef	__linux
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#endif

#include "danmaku.h"


DMWindow::DMWindow(int screenNumber) 
{

	QDesktopWidget desktop;
	QRect geo = desktop.screenGeometry(screenNumber);
	int sw = geo.width(), sh = geo.height();
	myDebug << sw << ", " << sh;

	this->resize(sw, sh);
	this->setWindowTitle("Danmaku");
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowFlags(Qt::ToolTip|Qt::FramelessWindowHint);
	this->show();
	this->move(geo.topLeft());
	this->init_slots();

#ifdef __linux
	QRegion region;
	XShapeCombineRegion(
			QX11Info::display(), this->winId(),
			ShapeInput, 0, 0, region.handle(), ShapeSet);
#endif

}

DMWindow::DMWindow(): DMWindow(0){};

void DMWindow::init_slots()
{
	int height = this->height();
	int nlines = (height - 2*VMARGIN) / LINE_HEIGHT_PX;
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

	Danmaku *l = new Danmaku(text, color, pos, slot, this);
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
