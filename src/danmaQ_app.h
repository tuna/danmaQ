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
