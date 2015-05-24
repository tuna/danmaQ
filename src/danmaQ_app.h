#ifndef __DANMAQ_APP_H__
#define __DANMAQ_APP_H__
#include <QWidget>
#include <QVector>
#include "subscriber.h"

class DMApp: public QWidget
{
	Q_OBJECT

public:
	DMApp(QString server, QString channel, QString passwd);

public slots:
	void reset_windows();
	void on_subscription_started();
	void on_subscription_stopped();
	void on_new_alert(QString msg);

private:
	QVector<QWidget*>	dm_windows;
	Subscriber *subscriber;
	void toggle_subscription(QString server, QString channel, QString passwd);
	void init_windows();


};

#endif
