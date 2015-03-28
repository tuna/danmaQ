#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QLabel>
#include <QWidget>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QDebug>

#include <cmath>
#include <map>
#include <utility>

#include "danmaku_ui.h"

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


Danmaku::Danmaku(QString text, QWidget *parent): Danmaku(text, "blue", "fly", parent){};

Danmaku::Danmaku(QString text, QString color, QString position, QWidget *parent)
	:QLabel(escape_text(text), parent)
{
	QString tcolor = colormap[color].first;
	QColor bcolor = colormap[color].second;
	QString style = style_tmpl
		.arg(20)
		.arg("WenQuanYi Micro Hei")
		.arg(tcolor);

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
	effect->setBlurRadius(6);
	effect->setColor(bcolor);
	effect->setOffset(0, 0);

	this->setStyleSheet(style);
	this->setGraphicsEffect(effect);
	this->setContentsMargins(0, 0, 0, 0);
	
	QSize _msize = this->minimumSizeHint();
	this->resize(_msize);

	this->_position = position;
	this->init_position();
}


QString Danmaku::escape_text(QString text) {
	return text;
}

QString Danmaku::style_tmpl = QString(
		"font-size: %1pt;"
		"font-weight: bold;"
		"font-family: %2;"
		"color: %3;"
	);

void Danmaku::init_position() {
	int sw = this->parentWidget()->width();
	int sh = this->parentWidget()->height();

	if(this->_position.compare("fly") == 0) {
		qDebug() << "fly";
		this->_x = sw;
		this->_y = sh / 2;
		this->_step = (double)(sw + this->width()) / (10 * 1000 / this->_interval);
		this->fly();

	} else if (this->_position.compare("top") == 0) {
		qDebug() << "top";
	} else if (this->_position.compare("bottom") == 0) {
		qDebug() << "bottom";
	} else {
		qDebug() << "wrong position: " << this->_position;
	}
}

void Danmaku::fly() {
	int x = static_cast<int>(round(this->_x)), y = this->_y;
	this->_x -= this->_step;
	int x_dst = static_cast<int>(round(this->_x));

	if(x < -this->width()) {
		this->clean_close();
		return;
	}
	
	QTimer::singleShot(this->_interval, this, SLOT(fly()));
	if(x != x_dst) {
		this->move(x, y);
	}
}

void Danmaku::clean_close() {
	if(this->_exited)
		return;
	
	this->_exited = true;
	this->close();

	emit exited(this);
}



