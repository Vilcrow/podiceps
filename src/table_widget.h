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

#include "table_model.h"
#include <QSettings>
#include <QTableView>
#include <QWidget>

class DictionaryWidget;
class QSortFilterProxyModel;

class TableWidget : public QWidget {
    Q_OBJECT
public:
    enum Columns { OriginalColumn, TranslationColumn,
                   StatusColumn, DateColumn, CommentColumn };

    bool isSaved() const;
    void setSaved(bool value);

    void sortByColumn(int column, Qt::SortOrder order = Qt::AscendingOrder);

    bool addEntry(const WordLine &word);
    bool editEntry(const WordLine &word);
    void removeEntry();
    void fillTable(const QList<WordLine> words);
    void clear();

    void setFilter(int col = -1, const QRegExp &exp = QRegExp());
    int getRowCount() const;
    QString getColumnName(int col) const;

    QList<WordLine> getWords() const;
    WordLine getSelectedWord() const;

    QTableView* getTableView() const;

    void clearSelection();

    TableWidget(QWidget *parent = nullptr);
    virtual ~TableWidget();
signals:
    void dataChanged();
    void selectionChanged();
public slots:
    void updateSettings();
private slots:
    void openWordCard(const QModelIndex &index);
private:
    QSettings settings;
    bool changesSaved;
    TableModel *tableModel;
    QTableView *tableView;
    QSortFilterProxyModel *proxyModel;
};

#endif