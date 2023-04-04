/**************************************************************************/
/*  manual_widget.h                                                       */
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

#ifndef MANUAL_WIDGET_VIL_H
#define MANUAL_WIDGET_VIL_H

#include <QMap>
#include <QMainWindow>
#include <QTabWidget>
#include <QVector>

class QCloseEvent;
class QTextEdit;

class ManualWidget : public QMainWindow {
    Q_OBJECT
public:
    ManualWidget(QWidget *parent = nullptr);
    virtual ~ManualWidget();
signals:
    void windowClosed();
protected:
    void closeEvent(QCloseEvent *event);
private:
    enum Tabs { General, Dictionary, Preferences, Shortcuts };

    QTabWidget *tabWidget;
    QVector<Tabs> tabs;
    QMap<Tabs, QString> tabNames;
    QMap<Tabs, QString> tabContentFiles;

    void setupTabWidget();
    void setupTabs();
    void setupTextEdit(QWidget *widget, const QString &path);
};

#endif
