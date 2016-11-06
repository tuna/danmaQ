#ifndef __DANMAKU_SUBSCRIBER_H__
#define __DANMAKU_SUBSCRIBER_H__
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
