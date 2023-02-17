/**************************************************************************/
/*  dictionary_widget.cpp                                                 */
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

#include "dictionary_widget.h"
#include "main_window.h"
#include "table_model.h"
#include "save_dialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFile>

DictionaryWidget::DictionaryWidget()
						: dictionarySettings("Vilcrow", "podiceps2")
						, changesSaved(true)
{
	readSettings();
	mainLayout = new QVBoxLayout(this);
	setupTable();
	createLineEditWidgets();
	createButtons();
	if(!lastFileName.isEmpty())
		readFromFile(lastFileName);
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
	inputLayout = new QGridLayout();
	inputLayout->addWidget(originalLabel, 0, 0, Qt::AlignCenter);
	inputLayout->addWidget(translationLabel, 0, 1, Qt::AlignCenter);
	inputLayout->addWidget(statusLabel, 0, 2, Qt::AlignCenter);
	inputLayout->addWidget(dateLabel, 0, 3, Qt::AlignCenter);
	inputLayout->addWidget(originalLineEdit, 1, 0);
	inputLayout->addWidget(translationLineEdit, 1, 1);
	inputLayout->addWidget(statusLineEdit, 1, 2);
	inputLayout->addWidget(dateLineEdit, 1, 3);
	mainLayout->addLayout(inputLayout);
}

void DictionaryWidget::createButtons()
{
	buttonsLayout = new QHBoxLayout();
	addButton = new QPushButton(tr("Add"));
	connect(addButton, &QAbstractButton::clicked,
			this, &DictionaryWidget::addEntrySlot);
	buttonsLayout->addWidget(addButton);
	editButton = new QPushButton(tr("Edit"));
	editButton->setEnabled(false);
	connect(editButton, &QAbstractButton::clicked,
			this, &DictionaryWidget::editEntry);
	buttonsLayout->addWidget(editButton);
	findButton = new QPushButton(tr("Find"));
	connect(findButton, &QAbstractButton::clicked,
			this, &DictionaryWidget::findEntry);
	buttonsLayout->addWidget(findButton);
	deleteButton = new QPushButton(tr("Delete"));
	connect(deleteButton, &QAbstractButton::clicked,
			this, &DictionaryWidget::removeEntry);
	deleteButton->setEnabled(false);
	buttonsLayout->addWidget(deleteButton);
	mainLayout->addLayout(buttonsLayout);
}

void DictionaryWidget::setupTable()
{
	tableModel = new TableModel(this);
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(tableModel);
	proxyModel->setFilterKeyColumn(0);
	tableView = new QTableView;
	tableView->setModel(proxyModel);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->horizontalHeader()->setMinimumSectionSize(100);
	tableView->setColumnWidth(0, 150);
	tableView->setColumnWidth(1, 150);
	tableView->setColumnWidth(2, 100);
	tableView->verticalHeader()->hide();
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->setSortingEnabled(true);
	mainLayout->addWidget(tableView);
	connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
			this, &DictionaryWidget::updateActions);
}

void DictionaryWidget::readFromFile(const QString &fileName)
{
	if(fileName.isEmpty())
		return; //fix me
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		sendMessage(tr("Unable to open file: %1").arg(lastFileName));
		lastFileName = "";
		return;
	}
	QList<Word> words;
	try {
		QDataStream in(&file);
		in >> words;
	}
	catch(...) {
		QMessageBox::information(this, tr("Unable to open file"),
											tr("Invalid format"));
		return;
	}
	if(words.isEmpty())
		sendMessage(tr("The file no words"));
	else {
		//clean current table
		tableModel->removeRows(0, tableModel->rowCount(QModelIndex()),
														QModelIndex());
		for(const auto &word: qAsConst(words))
			addEntry(word.original, word.translation, word.status, word.date);
		lastFileName = fileName;
	}
	sendMessage(tr("The file \"%1\" is open").arg(lastFileName));
}

void DictionaryWidget::writeToFile(const QString &fileName)
{
	if(fileName.isEmpty())
		return; //fix me
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Unable to open file"),
											file.errorString());
		return;
	}
	QDataStream out(&file);
	out << tableModel->getWords();
	lastFileName = fileName;
	sendMessage(tr("The file \"%1\" saved").arg(lastFileName));
}

void DictionaryWidget::addEntry(QString original, QString translation,
								QString status, QString date)
{
	if(!tableModel->getWords().contains({ original, translation,
											status, date })) {
		tableModel->insertRows(0, 1, QModelIndex());
		QModelIndex index = tableModel->index(0, 0, QModelIndex());
		tableModel->setData(index, original, Qt::EditRole);
		index = tableModel->index(0, 1, QModelIndex());
		tableModel->setData(index, translation, Qt::EditRole);
		index = tableModel->index(0, 2, QModelIndex());
		tableModel->setData(index, status, Qt::EditRole);
		index = tableModel->index(0, 3, QModelIndex());
		tableModel->setData(index, date, Qt::EditRole);
		emit updateMenus();
	}
	else {
		emit sendMessage(tr("Duplicate word. " 
							"The word \"%1\" already exists.").arg(original));
	}
}

void DictionaryWidget::addEntrySlot()
{
	QString original = originalLineEdit->text();
	QString translation = translationLineEdit->text();
	QString status = statusLineEdit->text();
	if(status.isEmpty()) //user didn't specify the status
		status = tr("new");
	//get current date
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	if(!original.isEmpty()) {
		addEntry(original, translation, status, date);
		changesSaved = false;
		emit sendMessage(tr("Done"));
	}
	else
		emit sendMessage(tr("Enter the original word"));
}

void DictionaryWidget::editEntry()
{
	QString original = originalLineEdit->text();
	if(original.isEmpty()) {
		emit sendMessage(tr("Enter the original word"));
		return;
	}
	QString translation = translationLineEdit->text();
	QString status = statusLineEdit->text();
	QString date = ""; //formal variable
	QModelIndexList indexes = tableView->selectionModel()->selectedRows();
	int row = -1;
	for(auto index : indexes) {
		row = proxyModel->mapToSource(index).row();
		QModelIndex originalIndex = tableModel->index(row, 0, QModelIndex());
		if(original != tableModel->data(originalIndex, Qt::DisplayRole) &&
			tableModel->getWords().contains({ original, translation,
													status, date })) {
			emit sendMessage(tr("The word \"%1\" already exists.").arg(original));
			return;
	}
		tableModel->setData(originalIndex, original, Qt::EditRole);
		QModelIndex translationIndex = tableModel->index(row, 1, QModelIndex());
		tableModel->setData(translationIndex, translation, Qt::EditRole);
		QModelIndex statusIndex = tableModel->index(row, 2, QModelIndex());
		tableModel->setData(statusIndex, status, Qt::EditRole);
	}
	changesSaved = false;
	emit updateMenus();
}

void DictionaryWidget::findEntry()
{
	if(isEditLinesEmpty()) {
		proxyModel->setFilterRegExp("");
		proxyModel->setFilterKeyColumn(0);
		sendMessage(tr("The filter is cleared"));
		return;
	}
	int columnFind = -1; //which column will we search for
	QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
	QRegExp regExp;
	if(!originalLineEdit->text().isEmpty()) {
		regExp = QRegExp(originalLineEdit->text(), Qt::CaseInsensitive, syntax);
		columnFind = 0;
	}
	else if(!translationLineEdit->text().isEmpty()) {
		regExp = QRegExp(translationLineEdit->text(), Qt::CaseInsensitive,
																syntax);
		columnFind = 1;
	}
	else if(!statusLineEdit->text().isEmpty()) {
		regExp = QRegExp(statusLineEdit->text(), Qt::CaseInsensitive, syntax);
		columnFind = 2;
	}
	else if(!dateLineEdit->text().isEmpty()) {
		regExp = QRegExp(dateLineEdit->text(), Qt::CaseInsensitive, syntax);
		columnFind = 3;
	}
	proxyModel->setFilterKeyColumn(columnFind);
	proxyModel->setFilterRegExp(regExp);
	sendMessage(tr("The filter \"%1\" (%2) is set").arg(regExp.pattern())
					.arg(tableModel->headerData(columnFind, Qt::Horizontal,
					Qt::DisplayRole).toString()));
}

void DictionaryWidget::removeEntry()
{
	QModelIndexList indexes = tableView->selectionModel()->selectedRows();
	for(auto index : indexes) {
		int row = proxyModel->mapToSource(index).row();
		tableModel->removeRows(row, 1, QModelIndex());
	}
	changesSaved = false;
	emit updateMenus();
}

void DictionaryWidget::updateActions()
{
	QModelIndexList indexes = tableView->selectionModel()->selectedRows();
	if(!indexes.isEmpty()) {
		QString original, translation, status, date;
		int row = -1;
		for(auto index : indexes) {
			row = proxyModel->mapToSource(index).row();
			QModelIndex originalIndex = tableModel->index(row, 0,
													QModelIndex());
			QVariant varOriginal = tableModel->data(originalIndex,
													Qt::DisplayRole);
			original = varOriginal.toString();
			QModelIndex translationIndex = tableModel->index(row, 1,
													QModelIndex());
			QVariant varTranslation = tableModel->data(translationIndex,
													Qt::DisplayRole);
			translation = varTranslation.toString();
			QModelIndex statusIndex = tableModel->index(row, 2, QModelIndex());
			QVariant varStatus = tableModel->data(statusIndex, Qt::DisplayRole);
			status = varStatus.toString();
			QModelIndex dateIndex = tableModel->index(row, 3, QModelIndex());
			QVariant varDate = tableModel->data(dateIndex, Qt::DisplayRole);
			date = varDate.toString();
		}
		originalLineEdit->setText(original);	
		translationLineEdit->setText(translation);	
		statusLineEdit->setText(status);	
		dateLineEdit->setText(date);	
		deleteButton->setEnabled(true);
		editButton->setEnabled(true);
	}
	else { //dictionary is empty or the row don't selected
		originalLineEdit->setText("");	
		translationLineEdit->setText("");	
		statusLineEdit->setText("");	
		dateLineEdit->setText("");	
		deleteButton->setEnabled(false);
		editButton->setEnabled(false);
	}
}

bool DictionaryWidget::isEditLinesEmpty()
{
	if(originalLineEdit->text().isEmpty() &&
	   translationLineEdit->text().isEmpty() &&
	   statusLineEdit->text().isEmpty() &&
	   dateLineEdit->text().isEmpty())
		return true;
	return false;
}

void DictionaryWidget::clearInput()
{
	tableView->selectionModel()->clear();
	originalLineEdit->setText("");	
	translationLineEdit->setText("");	
	statusLineEdit->setText("");	
	dateLineEdit->setText("");	
	originalLineEdit->setFocus(Qt::OtherFocusReason);
}
//to import a text file of the podiceps
void DictionaryWidget::importFromFile(const QString &fileName)
{
	if(fileName.isEmpty())
		return; //fix me
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, tr("Unable to open file"),
											file.errorString());
		return;
	}
	QList<Word> words;
	QTextStream in(&file);
	while(!in.atEnd()) {
		QString line = in.readLine();
		QStringList list = line.split('|');
		if(list.size() != 5) { //first element is empty
			QMessageBox::information(this, tr("Incorrect string"),
												list.join('|'));
			return;
		}
		Word word;
		word.original = list[1]; //skip the empty element in list
		word.translation = list[2];
		word.status = list[3];
		//change format "dd-MM-yyyy" to "yyyy-MM-dd"
		QDateTime oldDate = QDateTime::fromString(list[4], "dd-MM-yyyy");
		QString newDate = oldDate.toString("yyyy-MM-dd");
		word.date = newDate;
		words << word;
	}
	if(words.isEmpty()) {
		QMessageBox::information(this, tr("No words in file"),
			tr("The file you are attempting to open contains no words."));
	}
	else {
		//clean current table
		tableModel->removeRows(0, tableModel->rowCount(QModelIndex()),
														QModelIndex());
		for(const auto &word: qAsConst(words))
			addEntry(word.original, word.translation, word.status, word.date);
	}
	changesSaved = false;
}
//to export to a text file in a "|original|translation|status|date\n" format
void DictionaryWidget::exportToFile(const QString &fileName)
{
	if(fileName.isEmpty())
		return; //fix me
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Unable to open file"),
											file.errorString());
		return;
	}
	QTextStream out(&file);
	QList<Word> words = tableModel->getWords();
	for(auto w : words) {
		//change format "yyyy-MM-dd" to "dd-MM-yyyy"
		QDateTime newDate = QDateTime::fromString(w.date, "yyyy-MM-dd");
		QString oldDate = newDate.toString("dd-MM-yyyy");
		out << QString("|%1|%2|%3|%4\n").arg(w.original)
						.arg(w.translation).arg(w.status).arg(oldDate);
	}
	file.close();
}

void DictionaryWidget::readSettings()
{
	dictionarySettings.beginGroup("/Settings");
	lastFileName = dictionarySettings.value("/last_file_name", "").toString();
	dictionarySettings.endGroup();
}

void DictionaryWidget::writeSettings()
{
	dictionarySettings.beginGroup("/Settings");
	dictionarySettings.setValue("last_file_name", lastFileName);
	dictionarySettings.endGroup();
}

DictionaryWidget::~DictionaryWidget()
{
	writeSettings();
}

void DictionaryWidget::createNewFile()
{
	//clean current table
	tableModel->removeRows(0, tableModel->rowCount(QModelIndex()),
													QModelIndex());
	lastFileName = "";
	clearInput();
}
