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

#ifndef DMMAINWINDOW_HPP
#define DMMAINWINDOW_HPP
#include <QAction>
#include <QApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QVector>
#include <QWidget>

class Subscriber;

class DMTrayIcon : public QSystemTrayIcon {
  Q_OBJECT

 public:
  DMTrayIcon(QWidget* parent = 0);
  QAction *showAction, *aboutAction, *exitAction, *refreshScreenAction;

 public slots:
  void on_activated(QSystemTrayIcon::ActivationReason e);
  void set_icon_running();
  void set_icon_stopped();

 private:
  QIcon icon_running, icon_stopped;
};

class DMMainWindow : public QWidget {
  Q_OBJECT

 public:
  DMMainWindow(QApplication* app);

  int lineHeight, fontSize, screenCount;
  QString fontFamily;
  float speedScale;

  QLineEdit* server;
  QPushButton *hideBtn, *mainBtn;

 public slots:
  void reset_canvases();
  void toggle_subscription();
  void on_subscription_started();
  void on_subscription_stopped();
  void on_new_alert(QString msg);
  void show_about_dialog();

 signals:
  void stop_subscription();

 private:
  QApplication* app;
  QVector<QWidget*> dm_canvases;
  QVector<QCheckBox*> screenBoxes;
  QWidget* screenWidget{};
  Subscriber* subscriber{};
  DMTrayIcon* trayIcon{};
  void init_canvases();
};

#endif
