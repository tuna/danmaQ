#include <QDesktopWidget>
#include "danmaku.h"

DMApp::DMApp(QString server, QString channel, QString passwd) {
	this->fontSize = 36;
	this->lineHeight = this->fontSize * 1.2;
	this->fontFamily = QString("WenQuanYi Micro Hei");
	this->speedScale = 1.0;
	
	this->subscriber = NULL;
	this->init_windows();
	this->toggle_subscription(server, channel, passwd);
}

void DMApp::toggle_subscription(QString server, QString channel, QString passwd) {
	
	if (this->subscriber == NULL || this->subscriber->isFinished())
	{
		this->subscriber = new Subscriber(server, channel, passwd, this);
		for(auto w=this->dm_windows.begin(); w != this->dm_windows.end(); ++w) {
			connect(
				this->subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
				*w, SLOT(new_danmaku(QString, QString, QString))
			);
		}
		connect(
			this->subscriber, SIGNAL(started()),
			this, SLOT(on_subscription_started())
		);
		connect(
			this->subscriber, SIGNAL(finished()),
			this, SLOT(on_subscription_stopped())
		);
		this->subscriber->start();

	} else {
		this->subscriber->terminate();
		this->subscriber = NULL;
	}

}

void DMApp::init_windows() {
	QDesktopWidget	desktop;
	
	for (int i=0; i<desktop.screenCount(); i++) {
		DMWindow* w = new DMWindow(i, this);
		this->dm_windows.append(w);

		if (!(this->subscriber == NULL || this->subscriber->isFinished())) {
			connect(
				this->subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
				w, SLOT(new_danmaku(QString, QString, QString))
		   );
		}
	}

}

void DMApp::reset_windows() {
	for(auto w=this->dm_windows.begin(); w != this->dm_windows.end(); ++w) {
		delete *w;
	}
	this->dm_windows.clear();
	this->init_windows();
}

void DMApp::on_subscription_started() {
	myDebug << "Subscription Started";
};
void DMApp::on_subscription_stopped() {
	myDebug << "Subscription Stopped";
};
void DMApp::on_new_alert(QString msg) {
	myDebug << "Alert:" << msg;
};
