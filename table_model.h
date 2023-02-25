/**************************************************************************/
/*  table_model.h                                                         */
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

#ifndef TABLE_MODEL_VIL_H
#define TABLE_MODEL_VIL_H

#include "word_line.h"
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QList>
#include <QString>

class TableModel : public QAbstractTableModel {
	Q_OBJECT
public:
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;

	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	bool insertRows(int position, int rows,
				    const QModelIndex &index = QModelIndex()) override;
	bool removeRows(int position, int rows,
					const QModelIndex &index = QModelIndex()) override;

	QList<WordLine> getWords() const { return words; };

	TableModel(QObject *parent = nullptr);
	TableModel(QList<WordLine> words, QObject *parent = nullptr);
private:
	QList<WordLine> words;
};

#endif