/*
 * This file is part of danmaQ.
 * 
 * DanmaQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DanmaQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore>
#include <QtGui/qtextdocument.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QVector>
#include <QRegExp>
#include <QDebug>

#ifdef	__linux
#include <QtX11Extras/qx11info_x11.h>
#include <X11/Xlib.h>
#include <X11/Xregion.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#endif

#include <cstdlib>

#include "common.hpp"


DMCanvas::DMCanvas(int screenNumber, DMMainWindow *parent)
{
	this->setParent(parent);
	this->mainWindow = parent;
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

	this->init_slots();
	
	this->show();
	this->move(geo.topLeft());

#ifdef __linux
	Region region = XCreateRegion();
	XShapeCombineRegion(
			QX11Info::display(), this->winId(),
			ShapeInput, 0, 0, region, ShapeSet);
#endif

}

DMCanvas::DMCanvas(DMMainWindow *parent): DMCanvas(0, parent){};

void DMCanvas::init_slots()
{
	int height = this->height();
	int nlines = (height - 2*VMARGIN) / (this->mainWindow->lineHeight);
	myDebug << nlines << this->mainWindow->lineHeight;
	for(int i=0; i<nlines; i++) {
		this->fly_slots.append(false);
		this->fixed_slots.append(false);
	}
}

int DMCanvas::allocate_slot(Position position) {
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

int DMCanvas::slot_y(int slot)
{
	return (this->mainWindow->lineHeight * slot + VMARGIN);
}

QString DMCanvas::escape_text(QString & text) {
	QString escaped = text.toHtmlEscaped();

	escaped.replace(QRegExp("([^\\\\])\\\\n"), "\\1<br/>");
	escaped.replace(QRegExp("\\\\\\\\n"), "\\n");
	escaped.replace(QRegExp("\\[s\\](.+)\\[/s\\]"), "<s>\\1</s>");

	return escaped;
}

void DMCanvas::new_danmaku(QString text, QString color, QString position)
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

	Danmaku *l = new Danmaku(escape_text(text), color, pos, slot, this, this->mainWindow);
	this->connect(l, &Danmaku::exited,
				  this, &DMCanvas::delete_danmaku);
	this->connect(l, &Danmaku::clear_fly_slot,
				this, &DMCanvas::clear_fly_slot);
	l->show();
	// l->move(200, 200);
}

void DMCanvas::clear_fly_slot(int slot) {
	myDebug << "Clear Flying Slot: " << slot;
	// myDebug << this->fly_slots;
	this->fly_slots[slot] = false;
}

void DMCanvas::delete_danmaku(Danmaku* dm) {
	if (dm->position == TOP || dm->position == BOTTOM) {
		this->fixed_slots[dm->slot] = false;
	}
	myDebug << "danmaku closed";
}

DMCanvas::~DMCanvas() {
	myDebug << "window closed";
}
