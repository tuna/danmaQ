#ifndef __DANMAQ_APP_H__
#define __DANMAQ_APP_H__
#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>

class Subscriber;

class DMApp: public QWidget
{
	Q_OBJECT

public:
	DMApp();

	int lineHeight, fontSize;
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

private:
	QVector<QWidget*>	dm_windows;
	Subscriber *subscriber;
	void init_windows();


};

#endif
