#ifndef __DANMAKU_WINDOW_H__
#define __DANMAKU_WINDOW_H__
#include <QWidget>
#include <QVector>
#include "danmaQ_app.h"
#include "danmaku_ui.h"

class DMWindow: public QWidget
{
	Q_OBJECT

public:
	DMWindow(DMApp *parent);
	DMWindow(int screenNumber, DMApp *parent);
	~DMWindow();
	DMApp *app;

	int slot_y(int slot);


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
