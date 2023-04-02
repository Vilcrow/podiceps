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
#include "action_add.h"
#include "action_delete.h"
#include "action_edit.h"
#include "header_menu.h"
#include "statistics_widget.h"
#include "word_add.h"
#include "word_edit.h"
#include <algorithm>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QRect>
#include <QSortFilterProxyModel>

bool TableWidget::isSaved() const
{
    return changesSaved;
}

void TableWidget::setSaved(bool value)
{
    changesSaved = value;
    if(!actionLog->isEmpty()) {
        actionLog->clear();
    }
}

void TableWidget::sortByColumn(int column, Qt::SortOrder order)
{
    tableView->sortByColumn(column, order);
}

// Adds a word if it isn't a duplicate, or returns false.
bool TableWidget::addEntry(const WordLine &word)
{
    if(!containsWord(word)) {
        tableModel->insertRows(0, 1, QModelIndex());

        QModelIndex index;

        index = tableModel->index(0, OriginalColumn, QModelIndex());
        tableModel->setData(index, word.getOriginal(), Qt::EditRole);

        index = tableModel->index(0, TranscriptionColumn, QModelIndex());
        tableModel->setData(index, word.getTranscription(), Qt::EditRole);

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

void TableWidget::editEntry() {
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    openWordEdit(indexes);

    clearSelection();
}

void TableWidget::deleteEntry()
{
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();

    if(indexes.isEmpty()) {
        qDebug() << "No rows selected";
        return;
    }

    for(QModelIndex i : indexes) {
        WordLine word = getWord(i);
        ActionDelete *act = new ActionDelete(word);
        makeLogEntry(act);
    }

    wordDeleteQueue.append(indexes);
    processQueues();
}

void TableWidget::deleteRow(const QModelIndex &index)
{
    int row = proxyModel->mapToSource(index).row();
    tableModel->removeRows(row, 1, QModelIndex());
}

void TableWidget::fillTable(const QList<WordLine> words)
{
    if(words.isEmpty()) {
        qDebug() << "Passed an empty argument";
        return;
    }

    clear();

    int duplicates = 0;
    for(const WordLine &word: qAsConst(words)) {
        if(!addEntry(word)) {
            ++duplicates;
        }
    }

    if(duplicates != 0) {
        QString msg = tr("Duplicate words were found in the file(%1). "
                         "When you save the file, they will be lost.")
                                                      .arg(duplicates);
        QMessageBox::information(this, tr("Duplicate words"), msg);
    }
}

void TableWidget::clear()
{
    tableModel->removeRows(0, tableModel->rowCount(QModelIndex()),
                           QModelIndex());
    if(!actionLog->isEmpty()) {
        actionLog->clear();
    }
}

void TableWidget::setFilter(int col, const QString &filter)
{
    QRegExp::PatternSyntax syn = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp exp = QRegExp(filter, Qt::CaseInsensitive, syn);

    proxyModel->setFilterRegExp(exp);
    proxyModel->setFilterKeyColumn(col);
}

int TableWidget::getRowCount() const
{
    return tableModel->rowCount(QModelIndex());
}

QString TableWidget::getColumnName(int col) const
{
    return tableModel->headerData(col, Qt::Horizontal,
                                       Qt::DisplayRole).toString();
}

QList<WordLine> TableWidget::getWords() const
{
    return tableModel->getWords();
}

bool TableWidget::hasSelectedWords() const
{
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    if(indexes.isEmpty()) {
        return false;
    }

    return true;
}

WordLine TableWidget::getWord(const QModelIndex &index) const
{
    WordLine ret;

    int row = proxyModel->mapToSource(index).row();

    if(row == -1) {
        qDebug() << "Passed an invalid argument";
        return ret;
    }

    QModelIndex idx;
    QVariant value;

    idx = tableModel->index(row, OriginalColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString original = value.toString();

    idx = tableModel->index(row, TranscriptionColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString transcription = value.toString();

    idx = tableModel->index(row, TranslationColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString translation = value.toString();

    idx = tableModel->index(row, StatusColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString status = value.toString();

    idx = tableModel->index(row, DateColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString date = value.toString();

    idx = tableModel->index(row, CommentColumn, QModelIndex());
    value = tableModel->data(idx, Qt::DisplayRole);
    QString comment = value.toString();

    ret = WordLine(original, translation, status, date);
    ret.setTranscription(transcription);
    ret.setComment(comment);

    return ret;
}

bool TableWidget::containsWord(const WordLine &word) const
{
    return tableModel->getWords().contains(word);
}

QTableView* TableWidget::getTableView() const
{
    return tableView;
}

void TableWidget::clearSelection()
{
    tableView->clearSelection();
}

void TableWidget::addWord(const WordLine &word, bool addToLog)
{
    QString original = word.getOriginal();
    if(word.isEmpty()) {
        openWordAdd();
    }
    else if(original.isEmpty()) {
        openWordAdd(word, tr("Enter the original word"));
    }
    else {
        bool isDuplicate = !addEntry(word);
        if(isDuplicate) {
            QString msg = tr("The word \"%1\" already exists.")
                             .arg(original);
            openWordAdd(word, msg);
        }
        else if(addToLog) {
            ActionAdd *act = new ActionAdd(word);
            makeLogEntry(act);
            emit actionCompleted(tr("Added"));
        }
    }
}

void TableWidget::deleteWord(const WordLine &word, bool addToLog)
{
    int rows = tableModel->rowCount(QModelIndex());
    QModelIndex idx;
    QString original = word.getOriginal();

    for(int row = 0; row < rows; ++row) {
        idx = tableModel->index(row, TableWidget::OriginalColumn, QModelIndex());
        if(tableModel->data(idx, Qt::DisplayRole) == original) {
            tableModel->removeRows(row, 1, QModelIndex());
            if(addToLog) {
                ActionDelete *act = new ActionDelete(word);
                makeLogEntry(act);
            }
            break;
        }
    }
}

void TableWidget::updateSettings()
{
    settings.beginGroup("/Settings/Interface");
    bool hideTranscription = !settings.value("/table/show_transcription", true).toBool();
    bool hideStatus = !settings.value("/table/show_status", true).toBool();
    bool hideDate = !settings.value("/table/show_date", true).toBool();
    settings.endGroup();

    visibleColumns = TableModel::ColumnCount - 1;

    if(hideTranscription) {
        --visibleColumns;
    }
    if(hideStatus) {
        --visibleColumns;
    }
    if(hideDate) {
        --visibleColumns;
    }

    int widthOne = width() / visibleColumns;
    tableView->setColumnWidth(OriginalColumn, widthOne);
    tableView->setColumnWidth(TranscriptionColumn, widthOne);
    tableView->setColumnWidth(TranslationColumn, widthOne);
    tableView->setColumnWidth(StatusColumn, widthOne);

    tableView->setColumnHidden(TranscriptionColumn, hideTranscription);
    tableView->setColumnHidden(StatusColumn, hideStatus);
    tableView->setColumnHidden(DateColumn, hideDate);
}

void TableWidget::openWordAdd(const WordLine &word, const QString &msg)
{
    WordLine w;

    if(word.isEmpty()) {
        w.setCurrentDate();
    }
    else {
        w = word;
    }

    WordAdd *wordAdd = new WordAdd(w, this);
    if(!msg.isEmpty()) {
        wordAdd->showMessage(msg);
    }

    while(true) {
        int result = wordAdd->exec();
        WordLine word = wordAdd->getWord();
        if(result == QDialog::Rejected) {
            break;
        }
        else if(!word.getOriginal().isEmpty()) {
            if(addEntry(word)) {
                ActionAdd *act = new ActionAdd(word);
                makeLogEntry(act);
                break;
            }
            else {
                wordAdd = new WordAdd(word, this);
                QString msg = tr("The word \"%1\" already exists.")
                                 .arg(wordAdd->getWord().getOriginal());
                wordAdd->showMessage(msg);
                continue;
            }
        }
        else {
            wordAdd = new WordAdd(word, this);
            wordAdd->showMessage(tr("Enter the original word"));
            continue;
        }
    }
}

void TableWidget::openWordEdit(const QModelIndexList &indexes)
{
    QList<int> rows;
    if(indexes.isEmpty()) {  // Without selection.
        for(int i = 0; i < tableModel->rowCount(QModelIndex()); ++i) {
            QModelIndex idx = proxyModel->index(i, 0);
            rows.append(proxyModel->mapToSource(idx).row());
        }
    }
    else {
        for(int i = 0; i < indexes.size(); ++i) {
            rows.append(proxyModel->mapToSource(indexes[i]).row());
        }
    }

    if(rows.isEmpty()) {
        qDebug() << "The file is empty";
        return;
    }

    QList<WordLine> words = tableModel->getWords();
    WordEdit wordEdit(words, rows, this);
    if(wordEdit.exec() == QDialog::Accepted) {
        fillTable(wordEdit.getWords());
        emit actionCompleted(tr("Changes are fixed"));
    }
    else {
        emit actionCompleted(tr("Changes canceled"));
    }
}

void TableWidget::openStatistics()
{
    int totalCount = getRowCount();
    int newCount = tableModel->getWordsCount(WordStatus::New);
    int middleCount = tableModel->getWordsCount(WordStatus::Middle);
    int learnedCount = tableModel->getWordsCount(WordStatus::Learned);

    StatisticsWidget statisticsWidget(this);
    statisticsWidget.setAll(totalCount, newCount, middleCount, learnedCount);
    statisticsWidget.exec();
}

void TableWidget::resize(int w, int h)
{
    Q_UNUSED(h);

    int widthOne = w / visibleColumns;

    tableView->setColumnWidth(OriginalColumn, widthOne);
    tableView->setColumnWidth(TranscriptionColumn, widthOne);
    tableView->setColumnWidth(TranslationColumn, widthOne);
    tableView->setColumnWidth(StatusColumn, widthOne);

    QRect g = geometry();
    g.setWidth(w);
    setGeometry(g);
}

void TableWidget::undo()
{
    actionLog->undo();
}

void TableWidget::redo()
{
    actionLog->redo();
}

void TableWidget::rowDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    openWordEdit(indexes);

    processQueues();
    clearSelection();
}

void TableWidget::openContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    QAction *addAction = new QAction(tr("&Add"), this);
    QAction *editAction = new QAction(tr("&Edit"), this);
    QAction *deleteAction = new QAction(tr("Delete"), this);

    QModelIndex index = tableView->indexAt(pos);
    // Row clicked.
    if(index.isValid()) {
        menu->addAction(editAction);
        menu->addAction(deleteAction);
        menu->addSeparator();
    }

    menu->addAction(addAction);

    connect(addAction, SIGNAL(triggered()), this, SLOT(openWordAdd()));
    connect(editAction, &QAction::triggered, this, &TableWidget::editEntry);
    connect(deleteAction, &QAction::triggered, this, &TableWidget::deleteEntry);

    QPoint position(mapToGlobal(pos));
    position.setY(position.y()+30);
    menu->exec(position);
}

void TableWidget::openHeaderContextMenu(const QPoint &pos)
{
    QPoint position(mapToGlobal(pos));
    position.setY(position.y()+20);
    HeaderMenu(position, this);

    updateSettings();
}

void TableWidget::processQueues()
{
    // Have there been any changes?
    bool changed = false;

    if(!wordDeleteQueue.isEmpty()) {
        std::sort(wordDeleteQueue.begin(), wordDeleteQueue.end());
        std::reverse(wordDeleteQueue.begin(), wordDeleteQueue.end());
        for(QModelIndex i : wordDeleteQueue) {
            deleteRow(i);
        }
        wordDeleteQueue.clear();
        changed = true;
        emit actionCompleted(tr("Deleted"));
    }

    if(changed) {
        changesSaved = false;
        emit dataChanged();
    }
}

// Tries to make a log entry. If it fails, deletes the passed action.
void TableWidget::makeLogEntry(ActionBase *act)
{
    if(!actionLog->addAction(act)) {
        delete act;
        act = nullptr;
    }
}

TableWidget::TableWidget(QWidget *parent)
    : QWidget(parent), settings("Vilcrow", "podiceps"),
      changesSaved(true), visibleColumns(TableModel::ColumnCount-1),
      wordDeleteQueue(QList<QModelIndex>()),
      tableModel(new TableModel(this)), tableView(new QTableView),
      proxyModel(new QSortFilterProxyModel(this)), actionLog(new ActionLog())
{
    proxyModel->setSourceModel(tableModel);
    proxyModel->setFilterKeyColumn(OriginalColumn);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    tableView->setModel(proxyModel);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setMinimumSectionSize(100);
    tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    tableView->setColumnWidth(OriginalColumn, 150);
    tableView->setColumnWidth(TranscriptionColumn, 150);
    tableView->setColumnWidth(TranslationColumn, 150);
    tableView->setColumnWidth(StatusColumn, 100);

    tableView->setColumnHidden(CommentColumn, true);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(OriginalColumn, Qt::AscendingOrder);

    updateSettings();

    connect(tableView, &QAbstractItemView::doubleClicked,
            this, &TableWidget::rowDoubleClicked);
    connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &TableWidget::selectionChanged);
    connect(tableView, &QWidget::customContextMenuRequested,
            this, &TableWidget::openContextMenu);
    connect(tableView->horizontalHeader(), &QWidget::customContextMenuRequested,
            this, &TableWidget::openHeaderContextMenu);
    connect(actionLog, &ActionLog::addWord,
            this, &TableWidget::addWord);
    connect(actionLog, &ActionLog::deleteWord,
            this, &TableWidget::deleteWord);
    connect(actionLog, &ActionLog::sendStatus,
            this, &TableWidget::actionCompleted);
}

TableWidget::~TableWidget()
{
    delete actionLog;
}
