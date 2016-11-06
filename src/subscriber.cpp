#include <QtCore>
#include <QEventLoop>
#include <QThread>
#include <QTimer>
#include <QNetworkReply>

#include <QUuid>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>

#include <QByteArray>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>

#include "danmaku.h"


Subscriber::Subscriber(QString server, QString channel, QString passwd, QObject* parent)
	: QThread(parent)
{
	this->server = server;
	this->channel = channel;
	this->passwd = passwd;
	
	QUuid uuid = QUuid::createUuid();
	this->_uuid = uuid.toString();
	// myDebug << this->_uuid;
	
	QUrl uri = QUrl((this->server+"/api/v1.1/channels/%1/danmaku").arg(this->channel));

	if(uri.scheme().compare("https") == 0) {
		uri.port(443);
	}
	
	request.setUrl(uri);
	// myDebug << uri;
	request.setRawHeader("X-GDANMAKU-SUBSCRIBER-ID", this->_uuid.toUtf8() );
	request.setRawHeader("X-GDANMAKU-AUTH-KEY", this->passwd.toUtf8() );
	
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void Subscriber::run() 
{
	mark_stop = false;
	
	http = new QNetworkAccessManager(NULL);

	QEventLoop loop;
	connect((DMApp *)this->parent(), SIGNAL(stop_subscription()),
			&loop, SLOT(quit()));
	
	// Set HTTP request timeout
	QTimer timeout;
	timeout.setSingleShot(true);
	
	while(1) {
		timeout.start(10000);
		QNetworkReply *reply = http->get(request);
		// If timeout signaled, let http request abort
		connect(&timeout, SIGNAL(timeout()), reply, SLOT(abort()));
		connect(reply, SIGNAL(finished()),
				 &loop, SLOT(quit()));
		loop.exec();
		timeout.stop();
		if(mark_stop) {
			myDebug << "Thread marked to stop";
			break;
		}
		if(reply->error()){
			myDebug << reply->errorString() << "Wait 2 secs";
			this->msleep(2000);
		} else {
			parse_response(reply);
		}
	}
	delete http;
	http = nullptr;
}


void Subscriber::parse_response(QNetworkReply* reply) {
	QVariant resp = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if(resp.isValid()) {
		bool fatal = false;
		int statusCode = resp.toInt();
		if (statusCode >= 400) {
			fatal = true;
			QString errMsg;
			if (statusCode == 403 ) {
				errMsg = "Wrong Password";
			} else if (statusCode == 404) {
				errMsg = "No Such Channel";
			} else if (statusCode >= 500) {
				errMsg = "Server Error";
			}
			myDebug << errMsg;
			emit new_alert(errMsg);
		}
		if (fatal) {
			return;
		}
	}
	
	// QByteArray json = QByteArray(
	// 		"[{\"text\": \"test\", \"style\": \"white\", \"position\": \"fly\"},"
	// 		"{\"text\": \"test2\", \"style\": \"white\", \"position\": \"fly\"}]"
	// );
	QJsonDocument json = QJsonDocument::fromJson(reply->readAll());

	if(json.isArray()) {
		QJsonArray dms = json.array();
		for(QJsonArray::iterator i = dms.begin(); i != dms.end(); ++i) {
			QJsonObject dm = (*i).toObject();
			QString text = dm["text"].toString(),
					color = dm["style"].toString(),
					position = dm["position"].toString();
			myDebug << text ;

			emit new_danmaku(text, color, position);
		}
	}
}

