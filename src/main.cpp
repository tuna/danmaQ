/*
 * This file is part of danmaQ.
 * 
 * DanmaQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DanmaQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QDesktopWidget>
#include <ctime>
#include <cstdlib>
#include <QDir>

#include "danmaQ_app.h"
#include "danmaku.h"

#if defined NDEBUG && (defined _WIN32 || defined __CYGWIN__)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(i18n);
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    std::srand(std::time(0));

	QDir dir(":translations");
	QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files,
										  QDir::Name);
	QStringListIterator i(fileNames);
	QString defaultLocale = QLocale::system().name();
	defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
	myDebug << defaultLocale;
	while (i.hasNext()) {
		QString file = i.next();
		QString current = file;
		current.truncate(current.lastIndexOf('.'));
		current.remove(0, current.indexOf('_') + 1);
		if (defaultLocale == current) {
			myDebug << "Using translation file " << dir.filePath(file);
			QTranslator *translator = new QTranslator();
			translator->load(dir.filePath(file));
			app.installTranslator(translator);
			break;
		}
	}

	QDesktopWidget* desktop = QApplication::desktop();
	DMApp* dm_app = new DMApp();
	app.connect(
		desktop, SIGNAL(workAreaResized(int)),
		dm_app, SLOT(reset_windows())
	);
	app.connect(
		desktop, SIGNAL(screenCountChanged(int)),
		dm_app, SLOT(reset_windows())
	);
	return app.exec();
}

