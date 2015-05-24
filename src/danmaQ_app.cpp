#include <QDesktopWidget>
#include "danmaku.h"

DMApp::DMApp(Subscriber *s) {
	this->subscriber = s;
	this->init_windows();
}

void DMApp::init_windows() {
	QDesktopWidget	desktop;
	
	for (int i=0; i<desktop.screenCount(); i++) {
		DMWindow* w = new DMWindow(i);
		this->dm_windows.append(w);
		this->connect(
			this->subscriber, SIGNAL(new_danmaku(QString, QString, QString)),
			w, SLOT(new_danmaku(QString, QString, QString))
		);
	}

}

void DMApp::reset_windows() {
	for(auto w=this->dm_windows.begin(); w != this->dm_windows.end(); ++w) {
		delete *w;
	}
	this->init_windows();
}
