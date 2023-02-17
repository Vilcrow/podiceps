/**************************************************************************/
/*  dictionary_widget.h                                                   */
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

#ifndef DICTIONARY_WIDGET_VIL_H
#define DICTIONARY_WIDGET_VIL_H

#include "table_model.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QHeaderView>
#include <QTextStream>
#include <QSettings>

class DictionaryWidget : public QWidget {
	Q_OBJECT
public:
	DictionaryWidget();
	virtual ~DictionaryWidget();
	void readFromFile(const QString &fileName);
	void writeToFile (const QString &fileName);
	void importFromFile(const QString &fileName);
	void exportToFile(const QString &fileName);
	void addEntry(QString original, QString translation,
					QString status, QString date);
	int getRowCount() const { return tableModel->rowCount(QModelIndex()); }
	bool isSaved() const { return changesSaved; }
	void setSaved(const bool flag) { changesSaved = flag; }
	QString getLastFileName() const { return lastFileName; }
	void setLastFileName(const QString &newLast) { lastFileName = newLast; }
	void readSettings();
	void writeSettings();
signals:
	void sendMessage(const QString &msg);
	void updateMenus();
public slots:
	void clearInput();
	void createNewFile();
private slots:
	void updateActions();
	void addEntrySlot();
	void editEntry();
	void findEntry();
	void removeEntry();
private:
	QSettings dictionarySettings;
	QString lastFileName;
	bool changesSaved;

	TableModel *tableModel;
	QTableView *tableView;
	QSortFilterProxyModel *proxyModel;

	QLabel *originalLabel;
	QLabel *translationLabel;
	QLabel *statusLabel;
	QLabel *dateLabel;

	QLineEdit *originalLineEdit;
	QLineEdit *translationLineEdit;
	QLineEdit *statusLineEdit;
	QLineEdit *dateLineEdit;

	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *findButton;
	QPushButton *deleteButton;

	QVBoxLayout *mainLayout;
	QGridLayout *inputLayout;
	QHBoxLayout *buttonsLayout;

	void createLineEditWidgets();
	void createButtons();
	void setupTable();
	bool isEditLinesEmpty();
};

#endif
