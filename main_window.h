/**************************************************************************/
/*  main_window.h                                                         */
/*                                                                        */
/*  vim:ts=4:sw=4:expandtab                                               */
/*                                                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                               podiceps2                                */
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

#ifndef MAIN_WINDOW_VIL_H
#define MAIN_WINDOW_VIL_H

#include "dictionary_widget.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QItemSelection>
#include <QStatusBar>
#include <QSettings>

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    void readSettings();
    void writeSettings();

    MainWindow();
    virtual ~MainWindow();
public slots:
    void updateActions();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void createFile();
    void openFile();
    bool saveChanges();
    bool trySaveChanges();
    void saveFile();
    void importFile(); // Import the podicepses .txt to binary.
    void exportFile();
    void showMessage(const QString &msg);
    void showStatistics();
    void openTutorial();
    void openAbout();
    void openPreferences();
    void quitApp();
private:
    void createMenus();
    void createFileMenu();
    void createEditMenu();
    void createToolsMenu();
    void createHelpMenu();

    bool closeImmediately;
    QSettings mainWindowSettings;
    DictionaryWidget *dictWidget;
    QStatusBar *statusBar;

    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *importAct;
    QAction *exportAct;
    QAction *exitAct;

    QMenu *editMenu;
    QAction *preferencesAct;

    QMenu *toolsMenu;
    QAction *showStatisticsAct;
    QAction *clearInputAct;

    QMenu *helpMenu;
    QAction *openTutorialAct;
    QAction *openAboutAct;
};

#endif
