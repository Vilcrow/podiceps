/**************************************************************************/
/*  table_model.cpp                                                       */
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

#include "table_model.h"
#include "table_widget.h"
#include <QDebug>
#include <QModelIndex>
#include <QColor>

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return words.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        qDebug() << "Passed an invalid argument";
        return QVariant();
    }

    if(index.row() >= words.size() || index.row() < 0) {
        qDebug() << "Passed an invalid argument";
        return QVariant();
    }

    if(role == Qt::DisplayRole) {
        const WordLine &word = words.at(index.row());
        if(index.column() == TableWidget::OriginalColumn) {
            return word.getOriginal();
        }
        else if(index.column() == TableWidget::TranscriptionColumn) {
            return word.getTranscription();
        }
        else if(index.column() == TableWidget::TranslationColumn) {
            return word.getTranslation();
        }
        else if(index.column() == TableWidget::StatusColumn) {
            return word.getStatusTranslation();
        }
        else if(index.column() == TableWidget::DateColumn) {
            return word.getDate();
        }
        else if(index.column() == TableWidget::CommentColumn) {
            return word.getComment();
        }
    }
    else if(role == Qt::BackgroundRole) {
        if(index.column() == TableWidget::StatusColumn) {
            const WordLine &word = words.at(index.row());
            WordStatus s(word.getStatus());
            return WordStatus::getColor(s.getStatusInt());
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal) {
        switch(section) {
        case TableWidget::OriginalColumn:
            return tr("Original");
        case TableWidget::TranscriptionColumn:
            return tr("Transcription");
        case TableWidget::TranslationColumn:
            return tr("Translation");
        case TableWidget::StatusColumn:
            return tr("Status");
        case TableWidget::DateColumn:
            return tr("Date");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) {
        qDebug() << "Passed an invalid argument";
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value,
                         int role)
{
    if(index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        WordLine word = words.value(row);
        if(index.column() == TableWidget::OriginalColumn) {
            word.setOriginal(value.toString());
        }
        else if(index.column() == TableWidget::TranscriptionColumn) {
            word.setTranscription(value.toString());
        }
        else if(index.column() == TableWidget::TranslationColumn) {
            word.setTranslation(value.toString());
        }
        else if(index.column() == TableWidget::StatusColumn) {
            word.setStatus(value.toString());
        }
        else if(index.column() == TableWidget::DateColumn) {
            word.setDate(value.toString());
        }
        else if(index.column() == TableWidget::CommentColumn) {
            word.setComment(value.toString());
        }
        else {
            return false;
        }
        words.replace(row, word);
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for(int row = 0; row < rows; ++row) {
        WordLine word;
        words.insert(position, word);
    }
    endInsertRows();

    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for(int row = 0; row < rows; ++row) {
        words.removeAt(position);
    }
    endRemoveRows();

    return true;
}

QList<WordLine> TableModel::getWords() const
{
    return words;
}

int TableModel::getWordsCount(WordStatus::Status status) const
{
    int newCount = 0;
    int middleCount = 0;
    int learnedCount = 0;
    for(const WordLine& w : words) {
        if(w.getStatusInt() == WordStatus::New) {
            ++newCount;
        }
        else if(w.getStatusInt() == WordStatus::Middle) {
            ++middleCount;
        }
        else if(w.getStatusInt() == WordStatus::Learned) {
            ++learnedCount;
        }
    }

    int ret = 0;

    switch(status) {
    case WordStatus::New:
        ret = newCount;
        break;
    case WordStatus::Middle:
        ret = middleCount;
        break;
    case WordStatus::Learned:
        ret = learnedCount;
        break;
    }

    return ret;
}

TableModel::TableModel(QList<WordLine> pWords, QObject *parent)
            : QAbstractTableModel(parent), words(pWords)
{

}

TableModel::~TableModel()
{

}
