#include <QApplication>
#include <QByteArray>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>
#include <qjson/parser.h> 

#include "danmaku.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DMWindow* w = new DMWindow();
	
	bool ok;
	QJson::Parser  parser;
	QByteArray json = QByteArray("[{\"text\": \"test\", \"style\": \"white\", \"position\": \"fly\"}]");
	QVariant res = parser.parse(json, &ok);

	if(ok) {
		QVariantList dms = res.toList();
		for(QVariantList::iterator i = dms.begin(); i != dms.end(); ++i) {
			QVariantMap dm = i->toMap();
			QString text = dm["text"].toString(),
					color = dm["style"].toString(),
					position = dm["position"].toString();
			w->new_danmaku(text, color, position);
		}
	}

	return app.exec();
}

