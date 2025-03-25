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

#ifndef DMCANVAS_HPP
#define DMCANVAS_HPP
#include <QVector>
#include <QWidget>
#include "DMMainWindow.hpp"
#include "Danmaku.hpp"

class DMCanvas : public QWidget {
  Q_OBJECT

 public:
  DMCanvas(DMMainWindow* parent);
  DMCanvas(QScreen* screen, DMMainWindow* parent);
  ~DMCanvas();
  DMMainWindow* mainWindow;

  int slot_y(int slot);
  QRect screen;
  QPoint getGlboalPoint(QPoint p);

 public slots:
  void new_danmaku(QString text, int color, int position);
  void delete_danmaku(Danmaku*);

 private:
  QVector<bool> fly_slots, fixed_slots;
  void init_slots();
  int allocate_slot(Position);
  QString escape_text(QString&);
};

#endif
