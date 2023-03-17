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

#include "word_line.h"
#include <QSettings>
#include <QWidget>

class FindWidget;
class InputWidget;
class TableWidget;

class DictionaryWidget : public QWidget {
    Q_OBJECT
public:
    void readFromFile(const QString &fileName);
    void writeToFile (const QString &fileName);
    bool writeToXmlFile(const QString &fileName);
    void setLastFileName(const QString &newLast);

    bool isSaved() const;
    void setSaved(bool value);

    QString getLastFileName() const;
    int getRowCount() const;

    bool hasSelectedWords() const;

    void readSettings();
    void writeSettings();

    DictionaryWidget(QWidget *parent = nullptr);
    virtual ~DictionaryWidget();
signals:
    void actionCompleted(const QString &msg);
    void stateChanged();

    void addWordRequested(const WordLine& word = WordLine());
    void editWordRequested();
    void deleteWordRequested();
    void undoRequested();
    void redoRequested();
public slots:
    void addWord(const WordLine& word);
    void editWord();
    void deleteWord();

    void createNewFile();
    void clearInput();
    void setFilter();
    void clearFilter();
    void openFindWidget();
    void closeFindWidget();
    void updateInput();
    void updateSettings();
    void resize(int w, int h);
private:
    QSettings settings;
    QString lastFileName;

    TableWidget *tableWidget;
    FindWidget *findWidget;
    InputWidget *inputWidget;
};

#endif
