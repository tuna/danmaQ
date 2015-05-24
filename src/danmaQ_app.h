#ifndef __DANMAQ_APP_H__
#define __DANMAQ_APP_H__
#include <QWidget>
#include <QVector>
#include "subscriber.h"

class DMApp: public QWidget
{
	Q_OBJECT

public:
	DMApp(Subscriber *s);

public slots:
	void reset_windows();

private:
	QVector<QWidget*>	dm_windows;
	Subscriber *subscriber;
	void init_windows();


};

#endif
