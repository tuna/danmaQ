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

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QScreen>
#include <QTranslator>
#include <QtGlobal>

#include <cstdlib>
#include <ctime>

#include "DMMainWindow.hpp"
#include "common.hpp"

#if defined NDEBUG && (defined _WIN32 || defined __CYGWIN__)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  std::srand(std::time(0));

  QDir dir(":/i18n");
  QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);
  myDebug << "Available translations:" << fileNames;

  QString systemLocale = QLocale::system().name();
  myDebug << "System locale:" << systemLocale;

  // if system language is zh_CN, then first look for zh_CN, then zh_*
  QRegularExpression filter(QString("^(%1|%2.*)$").arg(systemLocale).arg(systemLocale.split('-').at(0)));
  auto availableTranslation = fileNames.filter(filter);
  if (availableTranslation.length() > 0) {
    QString translationPath = dir.filePath(availableTranslation.at(0));
    myDebug << "Using translation:" << translationPath;
    QTranslator* translator = new QTranslator();
    translator->load(translationPath);
    app.installTranslator(translator);
  } else {
    myDebug << "No available translation found.";
  }

  auto dmMainWindow = new DMMainWindow(&app);
  app.connect(&app, &QApplication::screenAdded, dmMainWindow, &DMMainWindow::reset_canvases);
  app.connect(&app, &QApplication::screenRemoved, dmMainWindow, &DMMainWindow::reset_canvases);
  app.setQuitOnLastWindowClosed(false);
  return app.exec();
}
