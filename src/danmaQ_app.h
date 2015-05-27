#ifndef __DANMAQ_APP_H__
#define __DANMAQ_APP_H__
#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class Subscriber;

class DMTrayIcon: public QSystemTrayIcon
{
	Q_OBJECT

public:
	DMTrayIcon(QWidget *parent=0);
	QAction *toggleAction, *showAction, *aboutAction, *exitAction,
			*refreshScreenAction;


public slots:
	void on_activated(QSystemTrayIcon::ActivationReason e);
	void set_icon_running();
	void set_icon_stopped();

private:
	QIcon icon_running, icon_stopped;

};


class DMApp: public QWidget
{
	Q_OBJECT

public:
	DMApp();

	int lineHeight, fontSize, screenCount;
	QString fontFamily;
	float speedScale;

	QLineEdit *server, *channel, *passwd;
	QPushButton *configBtn, *hideBtn, *mainBtn;
	

public slots:
	void reset_windows();
	void toggle_subscription();
	void on_subscription_started();
	void on_subscription_stopped();
	void on_new_alert(QString msg);
	void show_about_dialog();

signals:
	void stop_subscription();

private:
	QVector<QWidget*>	dm_windows;
	Subscriber *subscriber;
	DMTrayIcon *trayIcon;
	void init_windows();


};

#endif
