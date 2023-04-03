/**************************************************************************/
/*  podices_application.h                                                 */
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

#ifndef PODICEPS_APPLICATION_VIL_H
#define PODICEPS_APPLICATION_VIL_H

#include "main_window.h"
#include <QApplication>

class PodicepsApplication : public QApplication {
    Q_OBJECT
public:
    int exec();

    PodicepsApplication(int &argc, char **argv);
    virtual ~PodicepsApplication();
public slots:
    void updateSettings();
private:
    MainWindow *mainWindow;

    void setTheme();
    void setLanguage();
};

#endif
