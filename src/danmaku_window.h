#ifndef __DANMAKU_WINDOW_H__
#define __DANMAKU_WINDOW_H__
#include <QWidget>
#include <QVector>
#include "danmaku_ui.h"

class DMWindow: public QWidget
{
	Q_OBJECT

public:
	DMWindow();
	DMWindow(int screenNumber);
	~DMWindow();

public slots:
	void new_danmaku(QString text, QString color, QString position);
	void delete_danmaku(Danmaku*);
	void clear_fly_slot(int slot);

private:
	QVector<bool> fly_slots, fixed_slots;
	void init_slots();
	int allocate_slot(Position);

};

#endif
