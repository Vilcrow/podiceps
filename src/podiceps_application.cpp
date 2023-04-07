/**************************************************************************/
/*  podiceps_application.hpp                                              */
/*                                                                        */
/*  vim:ts=4:sw=4:expandtab                                               */
/*                                                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                               podiceps                                 */
/*                                   -                                    */
/*                          pocket dictionary                             */
/**************************************************************************/
/* Copyright (C) 2022-present S.V.I. 'Vilcrow', <vilcrow.net>             */
/*                                                                        */
/* LICENCE:                                                               */
/* This program is free software: you can redistribute it and/or modify   */
/* it under the terms of the GNU General Public License as published by   */
/* the Free Software Foundation, either version 3 of the License, or      */
/* (at your option) any later version.                                    */
/*                                                                        */
/* This program is distributed in the hope that it will be useful,        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           */
/* GNU General Public License for more details.                           */
/* You should have received a copy of the GNU General Public License      */
/* along with this program. If not, see <http://www.gnu.org/licenses/>.   */
/**************************************************************************/

#include "podiceps_application.h"
#include "settings.h"
#include <QDebug>
#include <QTranslator>

int PodicepsApplication::exec()
{
    mainWindow->show();

    return QApplication::exec();
}

void PodicepsApplication::setTheme()
{
    QString theme = Settings::getThemeText();
    setStyleSheet(theme);
}

void PodicepsApplication::setLanguage()
{
    Settings::Language appLanguage = Settings::getLanguage();

    QTranslator *translator = new QTranslator;
    QString path;
    switch(appLanguage) {
    case Settings::RussianLang:
        path = ":/podiceps_ru.qm";
        break;
    default:
        path = "";
        break;
    }

    if(!path.isEmpty()) {
        if(translator->load(path)) {
            installTranslator(translator);
        }
        else {
            qDebug() << QString("Unable to open the translation file: %1")
                                                                .arg(path);
        }
    }
}

void PodicepsApplication::updateSettings()
{
    setTheme();
}

PodicepsApplication::PodicepsApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setApplicationName("podiceps");
    setApplicationVersion(PODICEPS_VERSION);
    setLanguage();  // Must be before creating the mainWindow.
    setTheme();

    mainWindow = new MainWindow;

    connect(mainWindow, &MainWindow::preferencesChanged,
            this, &PodicepsApplication::updateSettings);
}

PodicepsApplication::~PodicepsApplication()
{

}
