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
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTime>


class Subscriber : public QThread
{
	Q_OBJECT
	
public:
	Subscriber(QString server, QString channel, QString passwd, QObject* parent=0);
	void run();
	bool mark_stop;

public slots:
	void parse_response(QNetworkReply* reply);

signals:
	void new_danmaku(QString text, QString color, QString position);
	void new_alert(QString msg);

private:
	QNetworkAccessManager* http;
	QNetworkRequest request;
	QString server, channel, passwd, _uuid;
	
};


#endif 
