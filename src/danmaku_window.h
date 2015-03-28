#ifndef __DANMAKU_WINDOW_H__
#define __DANMAKU_WINDOW_H__
#include <QWidget>
#include "danmaku_ui.h"

class DMWindow: public QWidget
{
	Q_OBJECT

public:
	DMWindow();

public slots:
	void new_danmaku(QString text, QString color, QString position);
	void delete_danmaku(Danmaku*);

};

#endif
