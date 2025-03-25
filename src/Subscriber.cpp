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

#include "common.hpp"

Subscriber::Subscriber(QString server, QObject* parent) : QObject(parent) {
  this->server = server;
}

void Subscriber::start() {
  webView = new QWebEngineView;
  webChannel = new QWebChannel;
  webView->load(QUrl("qrc:socket.html"));
  webChannel->registerObject("context", this);
  webView->page()->setWebChannel(webChannel);
  connect(this, &Subscriber::finished, this, &Subscriber::deleteLater);
  emit started();
  myDebug << "started";
}

void Subscriber::finish() {
  webChannel->deleteLater();
  webView->deleteLater();
  emit finished();
  myDebug << "finished";
}

QString Subscriber::get_server() {
  return this->server;
}

void Subscriber::webError(QString text) {
  myDebug << text;
}

void Subscriber::show(QString text, int color, int position) {
  emit new_danmaku(text, color, position);
}

void Subscriber::connected() {
  emit new_danmaku(tr("Server connected"), 16777215, 5);
}

void Subscriber::disconnected() {
  emit new_danmaku(tr("Server disconnected"), 16777215, 5);
}
