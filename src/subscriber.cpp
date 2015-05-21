#include <QtCore>
#include <QEventLoop>
#include <QThread>

#include <QUuid>
#include <QHttp>
#include <QUrl>

#include <QByteArray>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>
#include <qjson/parser.h> 

#include "subscriber.h"


Subscriber::Subscriber(QString server, QString channel, QString passwd, QObject* parent)
	: QThread(parent)
{
	this->server = server;
	this->channel = channel;
	this->passwd = passwd;
	QUuid uuid = QUuid::createUuid();
	this->_uuid = uuid.toString();
	// qDebug() << this->_uuid;
	
	QString uri = QString("/api/v1.1/channels/%1/danmaku").arg(this->channel);
	QUrl baseUrl = QUrl(this->server);
	// qDebug() << baseUrl.host() << baseUrl.port();

	qint16 port = baseUrl.port(80);
	QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;

	if(baseUrl.scheme().compare("https") == 0) {
		port = baseUrl.port(443);
		mode = QHttp::ConnectionModeHttps;
	}
	
	
	http = new QHttp(baseUrl.host(), mode, port, this);
	header = QHttpRequestHeader("GET", uri);
	header.setValue("Host", baseUrl.host());
	header.setValue("X-GDANMAKU-SUBSCRIBER-ID", this->_uuid);
	header.setValue("X-GDANMAKU-AUTH-KEY", this->passwd);
	
	// connect(http, SIGNAL(requestFinished(int, bool)), &loop, SLOT(quit()));
	connect(http, SIGNAL(done(bool)), this, SLOT(parse_response(bool)));
}

void Subscriber::run() 
{
	QEventLoop loop;
	connect(http, SIGNAL(done(bool)), &loop, SLOT(quit()));
	
	while(1) {
		http->request(header);
		loop.exec();
		if(http->error()){
			qDebug() << http->errorString() << "Wait 2 secs";
			this->msleep(2000);
		}
	}
}


void Subscriber::parse_response(bool error) {
	if (error) {
		return;
	}
	
	bool ok;
	QJson::Parser  parser;
	
	// QByteArray json = QByteArray(
	// 		"[{\"text\": \"test\", \"style\": \"white\", \"position\": \"fly\"},"
	// 		"{\"text\": \"test2\", \"style\": \"white\", \"position\": \"fly\"}]"
	// );
	QByteArray json = http->readAll();

	QVariant res = parser.parse(json, &ok);

	if(ok) {
		QVariantList dms = res.toList();
		for(QVariantList::iterator i = dms.begin(); i != dms.end(); ++i) {
			QVariantMap dm = i->toMap();
			QString text = dm["text"].toString(),
					color = dm["style"].toString(),
					position = dm["position"].toString();
			qDebug() << text ;

			emit new_danmaku(text, color, position);
		}
	}
}

