#ifndef __DANMAKU_SUBSCRIBER_H__
#define __DANMAKU_SUBSCRIBER_H__
#include <QtCore>
#include <QThread>
#include <QTime>
#include <QHttp>


class Subscriber : public QThread
{
	Q_OBJECT
	
public:
	Subscriber(QString server, QString channel, QString passwd, QObject* parent=0);
	void run();
	bool mark_stop;

public slots:
	void parse_response(bool);

signals:
	void new_danmaku(QString text, QString color, QString position);
	void new_alert(QString msg);

private:
	QHttp* http;
	QHttpRequestHeader header;
	QString server, channel, passwd, _uuid;
	
};


#endif 
