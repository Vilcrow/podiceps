/*
podiceps2 - pocket dictionary

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <vilcrow.net>
--------------------------------------------------------------------------------
LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

TableModel::TableModel(QList<Word> words, QObject *parent)
			: QAbstractTableModel(parent), words(words)
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return words.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();
	if(index.row() >= words.size() || index.row() < 0)
		return QVariant();
	if(role == Qt::DisplayRole) {
		const auto &word = words.at(index.row());
		if(index.column() == 0)
			return word.original;
		else if(index.column() == 1)
			return word.translation;
		else if(index.column() == 2)
			return word.status;
		else if(index.column() == 3)
			return word.date;
	}
	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
								int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();
	if(orientation == Qt::Horizontal) {
		switch(section) {
		case 0:
			return tr("Original");
		case 1:
			return tr("Translation");
		case 2:
			return tr("Status");
		case 3:
			return tr("Date");
		default:
			return QVariant();
		}
	}
	return QVariant();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
	Q_UNUSED(index);
	beginInsertRows(QModelIndex(), position, position + rows - 1);
	for(int row = 0; row < rows; ++row)
		words.insert(position, { QString(), QString(), QString(), QString() });
	endInsertRows();
	return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position + rows - 1);
	for(int row = 0; row < rows; ++row)
		words.removeAt(position);
	endRemoveRows();
	return true;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role == Qt::EditRole) {
		int row = index.row();
		auto word = words.value(row);
		if(index.column() == 0)
			word.original = value.toString();
		else if(index.column() == 1)
			word.translation = value.toString();
		else if(index.column() == 2)
			word.status = value.toString();
		else if(index.column() == 3)
			word.date = value.toString();
		else
			return false;
		words.replace(row, word);
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList<Word> TableModel::getWords() const
{
	return words;
}
