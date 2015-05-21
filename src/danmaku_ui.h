#ifndef __DANMAKU_UI_H__
#define __DANMAKU_UI_H__

#include <QWidget>
#include <QLabel>
#include <QString>

enum Position { TOP=1, BOTTOM, FLY };

const int LINE_HEIGHT_PX = 42, LINE_HEIGHT_PT = 32;
const int VMARGIN = 20;

class Danmaku: public QLabel
{
Q_OBJECT

public:
	Danmaku(QString text, QString color, Position position, int slot, QWidget *parent=0);
	// Danmaku(QString text, QWidget *parent=0);
	Position position;
	int slot;

public slots:
	void fly();
	void clean_close();

signals:
	void exited(Danmaku*);
	void clear_fly_slot(int slot);

private:
	static QString style_tmpl;
	static const int _interval = 30;
	bool _exited = false;
	double _x, _step;
	int _y;
	
	static QString escape_text(QString text);
	void init_position();
	
};

#endif
