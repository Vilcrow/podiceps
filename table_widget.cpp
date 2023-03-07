/**************************************************************************/
/*  table_widget.cpp                                                      */
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

#include "table_widget.h"
#include "dictionary_widget.h"
#include "word_card.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>

bool TableWidget::isSaved() const
{
    return changesSaved;
}

void TableWidget::setSaved(bool value)
{
    changesSaved = value;
}

void TableWidget::clear()
{
    tableModel->removeRows(0, tableModel->rowCount(QModelIndex()),
                           QModelIndex());
}

void TableWidget::fillTable(const QList<WordLine> words)
{
    if(words.isEmpty()) {
        return;
    }

    clear();

    for(const WordLine &word: qAsConst(words)) {
        addEntry(word);
    }
}

bool TableWidget::addEntry(const WordLine &word)
{
    if(!tableModel->getWords().contains(word)) {
        tableModel->insertRows(0, 1, QModelIndex());
        QModelIndex index = tableModel->index(0, OriginalColumn, QModelIndex());
        tableModel->setData(index, word.getOriginal(), Qt::EditRole);
        index = tableModel->index(0, TranslationColumn, QModelIndex());
        tableModel->setData(index, word.getTranslation(), Qt::EditRole);
        index = tableModel->index(0, StatusColumn, QModelIndex());
        tableModel->setData(index, word.getStatus(), Qt::EditRole);
        index = tableModel->index(0, DateColumn, QModelIndex());
        tableModel->setData(index, word.getDate(), Qt::EditRole);

        index = tableModel->index(0, CommentColumn, QModelIndex());
        tableModel->setData(index, word.getComment(), Qt::EditRole);

        changesSaved = false;
        emit dataChanged();
        return true;
    }

    return false;
}

bool TableWidget::editEntry(const WordLine &word) {
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    int row = -1;
    for(QModelIndex index : indexes) {
        row = proxyModel->mapToSource(index).row();
        QModelIndex originalIndex = tableModel->index(row, OriginalColumn,
                                                      QModelIndex());

        if(word.getOriginal() != tableModel->data(originalIndex,
                                                  Qt::DisplayRole) &&
           tableModel->getWords().contains(word)) {
            return false;
        }

        tableModel->setData(originalIndex, word.getOriginal(), Qt::EditRole);
        QModelIndex translationIndex = tableModel->index(row, TranslationColumn,
                                                         QModelIndex());
        tableModel->setData(translationIndex, word.getTranslation(), Qt::EditRole);
        QModelIndex statusIndex = tableModel->index(row, StatusColumn,
                                                    QModelIndex());
        tableModel->setData(statusIndex, word.getStatus(), Qt::EditRole);

        QModelIndex commentIndex = tableModel->index(row, CommentColumn,
                                                    QModelIndex());
        tableModel->setData(commentIndex, word.getComment(), Qt::EditRole);
    }
    changesSaved = false;
    emit dataChanged();

    return true;
}

void TableWidget::removeEntry()
{
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    for(QModelIndex index : indexes) {
        int row = proxyModel->mapToSource(index).row();
        tableModel->removeRows(row, 1, QModelIndex());
    }
    changesSaved = false;
    emit dataChanged();
}

void TableWidget::setFilter(int col, const QRegExp &exp)
{
    proxyModel->setFilterRegExp(exp);
    proxyModel->setFilterKeyColumn(col);
}

int TableWidget::getRowCount() const
{
    return tableModel->rowCount(QModelIndex());
}

QList<WordLine> TableWidget::getWords() const
{
    return tableModel->getWords();
}

WordLine TableWidget::getSelectedWord() const
{
    WordLine ret;

    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    if(!indexes.isEmpty()) {
        QString original, translation, status, date, comment;
        int row = -1;
        for(QModelIndex index : indexes) {
            row = proxyModel->mapToSource(index).row();
            QModelIndex originalIndex = tableModel->index(row, OriginalColumn,
                                                          QModelIndex());
            QVariant varOriginal = tableModel->data(originalIndex,
                                                    Qt::DisplayRole);
            original = varOriginal.toString();
            QModelIndex translationIndex = tableModel->index(row,
                                                             TranslationColumn,
                                                             QModelIndex());
            QVariant varTranslation = tableModel->data(translationIndex,
                                                    Qt::DisplayRole);
            translation = varTranslation.toString();
            QModelIndex statusIndex = tableModel->index(row, StatusColumn,
                                                        QModelIndex());
            QVariant varStatus = tableModel->data(statusIndex, Qt::DisplayRole);
            status = varStatus.toString();
            QModelIndex dateIndex = tableModel->index(row, DateColumn,
                                                      QModelIndex());
            QVariant varDate = tableModel->data(dateIndex, Qt::DisplayRole);
            date = varDate.toString();

            QModelIndex commentIndex = tableModel->index(row, CommentColumn,
                                                      QModelIndex());
            QVariant varComment = tableModel->data(commentIndex, Qt::DisplayRole);
            comment = varComment.toString();
        }

        ret = WordLine(original, translation, status, date);
        ret.setComment(comment);
    }

    return ret;
}

QString TableWidget::getColumnName(int col) const
{
    return tableModel->headerData(col, Qt::Horizontal,
                                       Qt::DisplayRole).toString();
}

void TableWidget::connectSignals(DictionaryWidget *dictWidget)
{
    connect(this, &TableWidget::dataChanged,
            dictWidget, &DictionaryWidget::updateActions);
    connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            dictWidget, &DictionaryWidget::updateActions);
}

QTableView* TableWidget::getTableView() const
{
    return tableView;
}

void TableWidget::clearSelection()
{
    tableView->clearSelection();
}

void TableWidget::updateSettings()
{
    settings.beginGroup("/Settings/Inteface");
    bool hideStatus = !settings.value("/table/show_status", true).toBool();
    bool hideDate = !settings.value("/table/show_date", true).toBool();
    settings.endGroup();

    int visibleCount = 4;
    if(hideStatus) {
        --visibleCount;
    }
    if(hideDate) {
        --visibleCount;
    }

    int widthTotal = tableView->horizontalHeader()->length();
    int widthColumn = widthTotal / visibleCount;
    tableView->setColumnWidth(OriginalColumn, widthColumn);
    tableView->setColumnWidth(TranslationColumn, widthColumn);
    tableView->setColumnWidth(StatusColumn, widthColumn);

    tableView->setColumnHidden(StatusColumn, hideStatus);
    tableView->setColumnHidden(DateColumn, hideDate);
}

void TableWidget::openWordCard(const QModelIndex &index)
{
    WordLine word = getSelectedWord();
    WordCard wordCard(word, this);
    if(wordCard.exec() == QDialog::Accepted && word != wordCard.getWord()) {
        editEntry(wordCard.getWord());
        changesSaved = false;
        emit dataChanged();
    }
}

TableWidget::TableWidget(QWidget *parent)
    : QWidget(parent), settings("Vilcrow", "podiceps"),
      changesSaved(true)
{
    tableModel = new TableModel(this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(tableModel);
    proxyModel->setFilterKeyColumn(OriginalColumn);

    tableView = new QTableView;
    tableView->setModel(proxyModel);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setMinimumSectionSize(100);
    tableView->setColumnWidth(OriginalColumn, 150);
    tableView->setColumnWidth(TranslationColumn, 150);
    tableView->setColumnWidth(StatusColumn, 100);
    tableView->setColumnHidden(CommentColumn, true);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSortingEnabled(true);
    updateSettings();

    connect(tableView, &QAbstractItemView::doubleClicked,
            this, &TableWidget::openWordCard);
}

TableWidget::~TableWidget()
{

}
