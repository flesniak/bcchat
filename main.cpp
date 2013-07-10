/************************************************************************
 * Copyright (C) 2010 Fabian Lesniak <f.lesniak@gmx.de>                 *
 *                                                                      *
 * This file is part of BCChat.                                         *
 *                                                                      *
 * BCChat is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * BCChat is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with BCChat.  If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

#include <QApplication>
#include <QtCore/QTranslator>

#include "bcchat.h"

int main(int argc, char *argv[])
{
QApplication bcchat_app(argc, argv);

bcchat_app.setWindowIcon(QIcon(":icon-16.png"));

QTranslator translator_qt;
translator_qt.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
bcchat_app.installTranslator(&translator_qt);

QTranslator translator_bcchat;
translator_bcchat.load(QLocale::system().name(),":");
bcchat_app.installTranslator(&translator_bcchat);

//Main Window
bcchat window;
window.show();

return bcchat_app.exec();
}
