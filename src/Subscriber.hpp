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

#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include <QtCore>
#include <QWebEngineView>
#include <QtWebChannel>

class Subscriber : public QObject {
  Q_OBJECT

 public:
  Subscriber(QString server, QObject* parent = 0);
  void start();
  void finish();

 public slots:
  void show(QString text, int color, int position);
  void connected();
  void disconnected();
  QString get_server();
  void webError(QString text);

 signals:
  void new_danmaku(QString text, int color, int position);
  void new_alert(QString msg);
  void started();
  void finished();

 private:
  QWebEngineView* webView;
  QWebChannel* webChannel;
  QString server;
};

#endif
