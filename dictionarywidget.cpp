/*
podiceps2 - pocket dictionary

Copyright (C) 2022 S.V.I 'Vilcrow', <vilcrow.net>
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

#include <QMessageBox>
#include <QDateTime>
#include "dictionarywidget.h"
#include "mainwindow.h"

DictionaryWidget::DictionaryWidget()
{
	mainLayout = new QVBoxLayout(this);
	setupTable();
	createLineEditWidgets();
	createButtons();
}

void DictionaryWidget::createLineEditWidgets()
{
	originalLabel = new QLabel(tr("Original"));
	translationLabel = new QLabel(tr("Translation"));
	statusLabel = new QLabel(tr("Status"));
	dateLabel = new QLabel(tr("Date"));
	originalLineEdit = new QLineEdit();
	translationLineEdit = new QLineEdit();
	statusLineEdit = new QLineEdit();
	dateLineEdit = new QLineEdit();
	dateLineEdit->setReadOnly(true);
	gridLayout = new QGridLayout();
	gridLayout->addWidget(originalLabel, 0, 0, Qt::AlignCenter);
	gridLayout->addWidget(translationLabel, 0, 1, Qt::AlignCenter);
	gridLayout->addWidget(statusLabel, 0, 2, Qt::AlignCenter);
	gridLayout->addWidget(dateLabel, 0, 3, Qt::AlignCenter);
	gridLayout->addWidget(originalLineEdit, 1, 0);
	gridLayout->addWidget(translationLineEdit, 1, 1);
	gridLayout->addWidget(statusLineEdit, 1, 2);
	gridLayout->addWidget(dateLineEdit, 1, 3);
	mainLayout->addLayout(gridLayout);
}

void DictionaryWidget::createButtons()
{
	buttonsLayout = new QHBoxLayout();
	addButton = new QPushButton(tr("Add"));
	connect(addButton, &QAbstractButton::clicked,
			this, &DictionaryWidget::addEntrySlot);
	buttonsLayout->addWidget(addButton);
	editButton = new QPushButton(tr("Edit"));
	buttonsLayout->addWidget(editButton);
	mainLayout->addLayout(buttonsLayout);
}

void DictionaryWidget::setupTable()
{
	table = new TableModel(this);
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(table);
	proxyModel->setFilterKeyColumn(0);
	tableView = new QTableView;
	tableView->setModel(proxyModel);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->verticalHeader()->hide();
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->setSortingEnabled(true);
	mainLayout->addWidget(tableView);
}

void DictionaryWidget::readFromFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		return;
	}
	QList<Word> words;
	QDataStream in(&file);
	in >> words;
	if(words.isEmpty()) {
		QMessageBox::information(this, tr("No words in file"),
			tr("The file you are attempting to open contains no words."));
	}
	else {
		for(const auto &word: qAsConst(words))
			addEntry(word.original, word.translation, word.status, word.date);
	}
}

void DictionaryWidget::writeToFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		return;
	}
	QDataStream out(&file);
	out << table->getWords();
}

void DictionaryWidget::addEntry(QString original, QString translation,
								QString status, QString date)
{
	if(!table->getWords().contains({ original, translation, status, date })) {
		table->insertRows(0, 1, QModelIndex());
		QModelIndex index = table->index(0, 0, QModelIndex());
		table->setData(index, original, Qt::EditRole);
		index = table->index(0, 1, QModelIndex());
		table->setData(index, translation, Qt::EditRole);
		index = table->index(0, 2, QModelIndex());
		table->setData(index, status, Qt::EditRole);
		index = table->index(0, 3, QModelIndex());
		table->setData(index, date, Qt::EditRole);
	}
	else {
		QMessageBox::information(this, tr("Duplicate word"),
							tr("The word \"%1\" already exists.").arg(original));
	}
}

void DictionaryWidget::addEntrySlot()
{
	QString original = originalLineEdit->text();
	QString translation = translationLineEdit->text();
	QString status = statusLineEdit->text();
	QString date = QDateTime::currentDateTime().toString("dd-MM-yyyy");
	addEntry(original, translation, status, date);
}

void DictionaryWidget::editEntry()
{
	QString newWord = "333";
	QModelIndex index = table->index(0, 1, QModelIndex());
	table->setData(index, newWord, Qt::EditRole);
}
