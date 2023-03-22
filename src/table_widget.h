/**************************************************************************/
/*  table_widget.h                                                        */
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

#ifndef TABLE_WIDGET_VIL_H
#define TABLE_WIDGET_VIL_H

#include "action_log.h"
#include "table_model.h"
#include <QSettings>
#include <QTableView>
#include <QWidget>

class QSortFilterProxyModel;

class TableWidget : public QWidget {
    Q_OBJECT
public:
    enum Columns { OriginalColumn, TranscriptionColumn, TranslationColumn,
                   StatusColumn, DateColumn, CommentColumn };

    bool isSaved() const;
    void setSaved(bool value);

    void sortByColumn(int column, Qt::SortOrder order = Qt::AscendingOrder);

    bool addEntry(const WordLine &word);
    void editEntry();
    void deleteEntry();
    void deleteRow(const QModelIndex &index);
    void fillTable(const QList<WordLine> words);
    void clear();

    void setFilter(int col = -1, const QRegExp &exp = QRegExp());
    int getRowCount() const;
    QString getColumnName(int col) const;

    QList<WordLine> getWords() const;
    bool hasSelectedWords() const;
    WordLine getWord(const QModelIndex &index = QModelIndex()) const;
    bool containsWord(const WordLine &word) const;

    QTableView* getTableView() const;

    void clearSelection();

    TableWidget(QWidget *parent = nullptr);
    virtual ~TableWidget();
signals:
    void actionCompleted(const QString &msg);
    void dataChanged();
    void selectionChanged();
public slots:
    void addWord(const WordLine &word, bool addToLog = true);
    void deleteWord(const WordLine &word, bool addToLog = true);
    void updateSettings();
    void openWordAdd(const WordLine &word = WordLine(),
                     const QString &msg = QString());
    void openWordEdit(const QModelIndexList &indexes);
    void resize(int w, int h);
    void undo();
    void redo();
private slots:
    void rowDoubleClicked(const QModelIndex &index);
    void openContextMenu(const QPoint &pos);
    void openHeaderContextMenu(const QPoint &pos);
private:
    QSettings settings;
    bool changesSaved;
    int visibleColumns;

    QList<WordLine> wordAddQueue;
    QList<QModelIndex> wordDeleteQueue;

    TableModel *tableModel;
    QTableView *tableView;
    QSortFilterProxyModel *proxyModel;

    ActionLog *actionLog;

    void processQueues();
    void makeLogEntry(ActionBase *act);
};

#endif
