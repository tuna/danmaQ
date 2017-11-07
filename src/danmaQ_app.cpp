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

#include <QDesktopWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "danmaku.h"

DMApp::DMApp() {

	this->setWindowTitle("Danmaku");
	this->setWindowIcon(QIcon(":icon_active.png"));
	this->trayIcon = new DMTrayIcon(this);
	
	QVBoxLayout* layout = new QVBoxLayout(this);

	QHBoxLayout* hbox = new QHBoxLayout(this);
	hbox->addWidget(new QLabel("Server: ", this));
	this->server = new QLineEdit("https://dm.tuna.moe", this);
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
	this->configBtn->setEnabled(false);
	this->mainBtn = new QPushButton("&Subscribe", this);
	hbox->addWidget(this->hideBtn);
	hbox->addWidget(this->configBtn);
	hbox->addWidget(this->mainBtn);
	layout->addLayout(hbox);
	
	this->setLayout(layout);


	this->fontSize = 36;
	this->lineHeight = this->fontSize * 1.2;
	this->fontFamily = QString(
		"WenQuanYi Micro Hei, Source Han Sans CN, WenQuanYi Zen Hei,"
		"Microsoft YaHei, SimHei, "
		"STHeiti, Hiragino Sans GB, "
		"sans-serif"
	);
	this->speedScale = 1.0;
	
	this->subscriber = NULL;
	this->init_windows();

	connect(this->mainBtn, SIGNAL(released()), this, SLOT(toggle_subscription()));
	connect(this->hideBtn, SIGNAL(released()), this, SLOT(hide()));
	connect(this->trayIcon->toggleAction, SIGNAL(triggered()), this, SLOT(toggle_subscription()));
	connect(this->trayIcon->refreshScreenAction, SIGNAL(triggered()), this, SLOT(reset_windows()));
	connect(this->trayIcon->showAction, SIGNAL(triggered()), this, SLOT(show()));
	connect(this->trayIcon->aboutAction, SIGNAL(triggered()), this, SLOT(show_about_dialog()));
	connect(this->trayIcon->exitAction, SIGNAL(triggered()), this, SLOT(close()));


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
		connect(
			this->subscriber, SIGNAL(new_alert(QString)),
			this, SLOT(on_new_alert(QString))
		);
		this->subscriber->start();

	} else {
		this->subscriber->mark_stop = true;
		emit stop_subscription();
		if (this->subscriber->wait(1000) == false) {
			this->subscriber->terminate();
		}
		this->subscriber = NULL;
		this->reset_windows();
	}

}

void DMApp::init_windows() {
	QDesktopWidget desktop;
	this->screenCount = desktop.screenCount();
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
	myDebug << "Resetting windows";
	for(auto w=this->dm_windows.begin(); w != this->dm_windows.end(); ++w) {
		delete *w;
	}
	this->dm_windows.clear();
	this->init_windows();
}

void DMApp::on_subscription_started() {
	myDebug << "Subscription Started";
	this->hide();
	this->trayIcon->set_icon_running();
	this->mainBtn->setText("&Unsubscribe");
	this->trayIcon->showMessage("Subscription Started", "Let's Go");
}

void DMApp::on_subscription_stopped() {
	myDebug << "Subscription Stopped";
	this->trayIcon->set_icon_stopped();
	this->mainBtn->setText("&Subscribe");
}

void DMApp::on_new_alert(QString msg) {
	myDebug << "Alert:" << msg;
	this->trayIcon->showMessage("Ooops!", msg, QSystemTrayIcon::Critical);
	this->subscriber->mark_stop = true;
	emit stop_subscription();
	if (this->subscriber->wait(1000) == false) {
		this->subscriber->terminate();
	}
	this->subscriber = NULL;
}

void DMApp::show_about_dialog() {
	this->show();
	QMessageBox::about(
		this, "About", 
		"<strong>DanmaQ</strong>"
        "<p>Copyright &copy; 2015-2017 Justin Wong & TUNA members<br />"
		"Tsinghua University TUNA Association</p>"
		"<p> Source Code Available under GPLv3<br />"
        "<a href='https://github.com/tuna/danmaQ'>"
        "https://github.com/tuna/danmaQ"
		"</a></p>"
	);
}


DMTrayIcon::DMTrayIcon(QWidget *parent)
	:QSystemTrayIcon(parent)
{
	this->icon_running = QIcon(":icon_active.png");
	this->icon_stopped = QIcon(":icon_inactive.png");
	this->setIcon(this->icon_stopped);
	
	QMenu* menu = new QMenu(parent);
	this->toggleAction = menu->addAction("Toggle Subscription");
	this->refreshScreenAction = menu->addAction("Refresh Screen");
	this->showAction = menu->addAction("Show Main Window");
	this->aboutAction = menu->addAction("About");
	this->exitAction = menu->addAction("Exit");

	this->setContextMenu(menu);

	connect(
		this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(on_activated(QSystemTrayIcon::ActivationReason))
	);
	this->show();
}

void DMTrayIcon::on_activated(QSystemTrayIcon::ActivationReason e) {
	if(e == this->Trigger){
		QWidget *parent = (QWidget *)this->parent();
		if(parent == NULL) {
			return;
		}
		if(parent->isVisible()) {
			parent->hide();
		} else {
			parent->show();
		}
	}
}

void DMTrayIcon::set_icon_running() {
	this->setIcon(this->icon_running);
}

void DMTrayIcon::set_icon_stopped() {
	this->setIcon(this->icon_stopped);
}
