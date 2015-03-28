#ifndef __DANMAKU_UI_H__
#define __DANMAKU_UI_H__

#include <QWidget>
#include <QLabel>
#include <QString>

class Danmaku: public QLabel
{
Q_OBJECT

public:
	Danmaku(QString text, QString color, QString position, QWidget *parent=0);
	Danmaku(QString text, QWidget *parent=0);

public slots:
	void fly();

signals:
	void exited(Danmaku*);

private:
	static QString style_tmpl;
	static const int _interval = 30;
	bool _exited = false;
	QString _position;
	double _x, _step;
	int _y;
	
	static QString escape_text(QString text);
	void clean_close();
	void init_position();
	
};

#endif
