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
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QLabel>
#include <QWidget>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QAbstractAnimation>
#include <QPropertyAnimation>
#include <QRect>
#include <QTimer>
#include <QDebug>
#include <QString>

#include <cmath>
#include <map>
#include <utility>

#if defined _WIN32 || defined __CYGWIN__
#include <Windows.h>
#endif

#include "danmaku.h"


static std::map<QString, std::pair<QString, QColor>> colormap = {
	{"white", std::make_pair("rgb(255, 255, 255)", QColor("black"))},
	{"black", std::make_pair("rgb(0, 0, 0)", QColor("white"))},
	{"blue", std::make_pair("rgb(20, 95, 198)", QColor("white"))},
	{"cyan", std::make_pair("rgb(0, 255, 255)", QColor("black"))},
	{"red", std::make_pair("rgb(231, 34, 0)", QColor("white"))},
	{"yellow", std::make_pair("rgb(255, 221, 2)", QColor("black"))},
	{"green", std::make_pair("rgb(4, 202, 0)", QColor("black"))},
	{"purple", std::make_pair("rgb(128, 0, 128)", QColor("white"))}
};

// Danmaku::Danmaku(QString text, QWidget *parent): Danmaku(text, "blue", FLY, -1, parent){};

Danmaku::Danmaku(QString text, QString color, Position position, int slot, DMWindow *parent, DMMainWindow* mainWindow)
	:QLabel(escape_text(text), parent)
{
	this->dmwin = parent;
    this->app = mainWindow;
	this->setAttribute(Qt::WA_DeleteOnClose);

	QString tcolor = colormap[color].first;
	QColor bcolor = colormap[color].second;
	
	QString style = style_tmpl
		.arg(this->app->fontSize)
		.arg(this->app->fontFamily)
		.arg(tcolor);

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
	
	bool enableShadow = false;

#ifndef Q_WS_X11
	if (this->app->screenCount == 1) {
		this->setWindowFlags(
			Qt::ToolTip
			| Qt::FramelessWindowHint
		);
		this->setAttribute(Qt::WA_TranslucentBackground, true);
		this->setAttribute(Qt::WA_Disabled, true);
		this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
#if defined _WIN32 || defined __CYGWIN__
		// remove CS_DROPSHADOW
        SetClassLong((HWND)this->winId(), -26, 0x0008 & ~0x00020000);
#endif
		enableShadow = true;
	} else if (position != FLY) {
		enableShadow = true;
	}
#else
	enableShadow = true;
#endif
	
	myDebug << "enableShadow:" << enableShadow;

	if(enableShadow) {
		effect->setBlurRadius(6);
		effect->setColor(bcolor);
		effect->setOffset(0, 0);
		this->setGraphicsEffect(effect);
	}

	this->setStyleSheet(style);
	this->setContentsMargins(0, 0, 0, 0);
	
	QSize _msize = this->minimumSizeHint();
	this->resize(_msize);
	
	this->position = position;
	this->slot = slot;
	this->init_position();
}


QString Danmaku::escape_text(QString text) {
	return text;
}

QString Danmaku::style_tmpl = QString(
		"font-size: %1px;"
		"font-weight: bold;"
		"font-family: %2;"
		"color: %3;"
	);

void Danmaku::init_position() {
	int sw = this->parentWidget()->width();
	this->_y = this->dmwin->slot_y(this->slot);

	switch(this->position) {
		case FLY:
			myDebug << "fly";
			this->_x = sw;
			this->fly();
			break;
		case TOP:
		case BOTTOM:
			this->_x = (sw / 2) - (this->width() / 2);
			this->move(this->_x, this->_y);
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
			QTimer::singleShot(10 * 1000, this, &Danmaku::clean_close);
#else
			QTimer::singleShot(10 * 1000, this, SLOT(clean_close()));
#endif
			break;
	}
}

void Danmaku::fly() {
	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry", this);
	animation->setDuration(10 * 1000);
	animation->setStartValue(
			QRect(this->_x, this->_y, this->width(), this->height()));
	
	animation->setEndValue(
			QRect(-this->width(), this->_y, this->width(), this->height()));
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	
	connect(
		animation, &QPropertyAnimation::finished,
		this, &Danmaku::clean_close
	);
}

void Danmaku::clean_close() {
	if(this->position == FLY) {
		emit clear_fly_slot(this->slot);
	}
	
	this->close();
	emit exited(this);
}



