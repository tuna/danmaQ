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

#ifndef __DANMAKU_WINDOW_H__
#define __DANMAKU_WINDOW_H__
#include <QWidget>
#include <QVector>
#include "danmaQ_app.h"
#include "danmaku_ui.h"

class DMWindow: public QWidget
{
	Q_OBJECT

public:
    DMWindow(DMMainWindow *parent);
    DMWindow(int screenNumber, DMMainWindow *parent, int vertical_margin = VMARGIN);
	~DMWindow();
    DMMainWindow *app;

	int slot_y(int slot);


public slots:
	void new_danmaku(QString text, QString color, QString position);
	void delete_danmaku(Danmaku*);
	void clear_fly_slot(int slot);

private:
	QVector<bool> fly_slots, fixed_slots;
	int vertical_margin;
	void init_slots();
	int allocate_slot(Position);
	QString escape_text(QString &);

};

#endif
