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

#ifndef DANMAKU_HPP
#define DANMAKU_HPP

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVector>

enum Position
{
	UNUSED0 = 0,
	topScrolling,
	bottomScrolling,
	UNUSED3,
	bottomStatic,
	topStatic,
	topReverse
};
const QVector<QString> position_name = {"UNUSED0", "topScrolling", "bottomScrolling", "UNUSED3", "bottomStatic", "topStatic", "topReverse"};

const int VMARGIN = 20;

class DMCanvas;
class DMMainWindow;

class Danmaku: public QLabel
{
Q_OBJECT

public:
    Danmaku(QString text, int color, Position position, int slot, DMCanvas *parent, DMMainWindow *mainWindow);
	// Danmaku(QString text, QWidget *parent=0);
	Position position;
	int slot;
	DMCanvas *canvas;
    DMMainWindow *mainWindow;

public slots:
	void linearMotion(int startX, int startY, int endX, int endY);
	void clean_close();

signals:
	void exited(Danmaku*);

private:
	static QString style_tmpl;
	static QString escape_text(QString text);
	void init_position();
	
};

#endif
