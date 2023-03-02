/**************************************************************************/
/*  dictionary_widget.h                                                   */
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

#ifndef DICTIONARY_WIDGET_VIL_H
#define DICTIONARY_WIDGET_VIL_H

#include <QSettings>
#include <QWidget>

class InputWidget;
class TableWidget;
class WordLine;

class DictionaryWidget : public QWidget {
    Q_OBJECT
public:
    QString addEntry(const WordLine &word);

    void readFromFile(const QString &fileName);
    void writeToFile (const QString &fileName);
    void setLastFileName(const QString &newLast);
    void importFromFile(const QString &fileName);
    void exportToFile(const QString &fileName);

    bool isSaved() const;
    void setSaved(const bool value);

    QString getLastFileName() const;
    int getRowCount() const;

    void readSettings();
    void writeSettings();

    DictionaryWidget();
    virtual ~DictionaryWidget();
signals:
    void sendMessage(const QString &msg);
    void updateMenus();
public slots:
    void clearInput();
    void createNewFile();
    void addEntrySlot();
    void editEntry();
    void findEntry();
    void removeEntry();
    void updateActions();
private:
    QSettings dictionarySettings;
    QString lastFileName;
    bool changesSaved;

    TableWidget *tableWidget;
    InputWidget *inputWidget;
};

#endif
