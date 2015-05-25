#include <QDesktopWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "danmaku.h"

DMApp::DMApp() {

	this->setWindowTitle("Danmaku");
	
	QVBoxLayout* layout = new QVBoxLayout(this);
	QHBoxLayout* hbox = new QHBoxLayout(this);
	hbox->addWidget(new QLabel("Server: ", this));
	this->server = new QLineEdit("http://dm.tuna.moe", this);
	hbox->addWidget(this->server);
	layout->addLayout(hbox);
	
	hbox = new QHBoxLayout(this);
	hbox->addWidget(new QLabel("Channel: ", this));
	this->channel = new QLineEdit("demo", this);
	hbox->addWidget(this->channel);
	layout->addLayout(hbox);
	
	hbox = new QHBoxLayout(this);
	hbox->addWidget(new QLabel("Password: ", this));
	this->passwd = new QLineEdit("", this);
	this->passwd->setEchoMode(QLineEdit::Password);
	hbox->addWidget(this->passwd);
	layout->addLayout(hbox);

	hbox = new QHBoxLayout(this);
	this->hideBtn = new QPushButton("&Hide", this);
	this->configBtn = new QPushButton("&config", this);
	this->mainBtn = new QPushButton("&Subscribe", this);
	hbox->addWidget(this->hideBtn);
	hbox->addWidget(this->configBtn);
	hbox->addWidget(this->mainBtn);
	layout->addLayout(hbox);

	this->setLayout(layout);

	this->fontSize = 36;
	this->lineHeight = this->fontSize * 1.2;
	this->fontFamily = QString("WenQuanYi Micro Hei");
	this->speedScale = 1.0;
	
	this->subscriber = NULL;
	this->init_windows();

	connect(this->mainBtn, SIGNAL(released()), this, SLOT(toggle_subscription()));
	connect(this->hideBtn, SIGNAL(released()), this, SLOT(hide()));

	this->show();
	QDesktopWidget	desktop;
	auto center = desktop.screenGeometry(desktop.primaryScreen()).center();
	this->move(center.x() - this->width()/2, center.y() - this->height()/2);
}

void DMApp::toggle_subscription() {
	if (this->subscriber == NULL || this->subscriber->isFinished())
	{
		this->subscriber = new Subscriber(server->text(), channel->text(), passwd->text(), this);
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
