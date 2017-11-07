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

#ifndef __DANMAKU_UI_H__
#define __DANMAKU_UI_H__

#include <QWidget>
#include <QLabel>
#include <QString>

enum Position { TOP=1, BOTTOM, FLY };

const int VMARGIN = 20;

class DMWindow;
class DMApp;

class Danmaku: public QLabel
{
Q_OBJECT

public:
	Danmaku(QString text, QString color, Position position, int slot, DMWindow *parent, DMApp *app);
	// Danmaku(QString text, QWidget *parent=0);
	Position position;
	int slot;
	DMWindow *dmwin;
	DMApp *app;

public slots:
	void fly();
	void clean_close();

signals:
	void exited(Danmaku*);
	void clear_fly_slot(int slot);

private:
	static QString style_tmpl;
	int _x, _y;
	
	static QString escape_text(QString text);
	void init_position();
	
};

#endif
