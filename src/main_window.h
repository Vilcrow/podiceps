/**************************************************************************/
/*  main_window.h                                                         */
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

#ifndef MAIN_WINDOW_VIL_H
#define MAIN_WINDOW_VIL_H

#include <QMainWindow>
#include <QSettings>

class DictionaryWidget;
class QCloseEvent;
class QResizeEvent;
class QStatusBar;

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    static void appendFormat(QString &name, const QString &format);

    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
signals:
    void preferencesChanged();
    void resized(int w, int h);
public slots:
    void updateActions();
private slots:
    void openFile();
    void createFile();
    bool saveChanges();
    void saveFile();
    bool trySaveChanges();
    void showMessage(const QString &msg, int timeout = 5000);
    void showStatistics();
    void openTutorial();
    void openAbout();
    void openPreferences();
    void quitApp();
protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    QSettings mainWindowSettings;
    bool closeImmediately;
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
    QAction *undoAct;
    QAction *redoAct;
    QAction *addAct;
    QAction *findAct;
    QAction *editAct;
    QAction *deleteAct;
    QAction *preferencesAct;

    QMenu *toolsMenu;
    QAction *showStatisticsAct;
    QAction *clearInputAct;

    QMenu *helpMenu;
    QAction *openTutorialAct;
    QAction *openAboutAct;

    void createMenus();
    void createFileMenu();
    void createEditMenu();
    void createToolsMenu();
    void createHelpMenu();

    void readSettings();
    void writeSettings();
};

#endif
