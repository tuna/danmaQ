#ifndef __DANMAKU_UI_H__
#define __DANMAKU_UI_H__

#include <QWidget>
#include <QLabel>
#include <QString>

enum Position { TOP=1, BOTTOM, FLY };

const int VMARGIN = 20;

class DMWindow;
class DMApp;

class Danmaku: public QLabel
{
Q_OBJECT

public:
	Danmaku(QString text, QString color, Position position, int slot, DMWindow *parent, DMApp *app);
	// Danmaku(QString text, QWidget *parent=0);
	Position position;
	int slot;
	DMWindow *dmwin;
	DMApp *app;

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
