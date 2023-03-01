/**************************************************************************/
/*  dictionary_widget.cpp                                                 */
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

#include "dictionary_widget.h"
#include "save_dialog.h"
#include "word_line.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

void DictionaryWidget::readFromFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        sendMessage(tr("Unable to open file: %1").arg(lastFileName));
        lastFileName = "";
        return;
    }

    QList<WordLine> words;
    try {
        QDataStream in(&file);
        in >> words;
    }
    catch(...) {
        QMessageBox::information(this, tr("Unable to open file"),
                                       tr("Invalid format"));
        file.close();
        return;
    }

    if(words.isEmpty()) {
        sendMessage(tr("The file is empty"));
    }
    else {
        tableWidget->fillTable(words);
        lastFileName = fileName;
    }

    changesSaved = true;
    file.close();
    sendMessage(tr("The file \"%1\" is open").arg(lastFileName));
}

void DictionaryWidget::writeToFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream out(&file);
    out << tableWidget->getWords();

    lastFileName = fileName;
    changesSaved = true;

    file.close();
    sendMessage(tr("The file \"%1\" saved").arg(lastFileName));
}

void DictionaryWidget::setLastFileName(const QString &newLast)
{
    lastFileName = newLast;
}

// To import a text file of the 'podiceps-fltk'.
void DictionaryWidget::importFromFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QList<WordLine> words;
    QTextStream in(&file);
    QString oldDateFormat = "dd-MM-yyyy";
    QString newDateFormat = "yyyy-MM-dd";
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList list = line.split('|');
        if(list.size() != 5) { // First element is empty.
            QMessageBox::information(this, tr("Incorrect string"),
                                     list.join('|'));
            return;
        }
        WordLine word(list[1], list[2], list[3], list[4], oldDateFormat);
        word.setDateFormat(newDateFormat);
        words << word;
    }

    if(words.isEmpty()) {
        QMessageBox::information(this, tr("No words in file"),
            tr("The file you are attempting to open contains no words."));
    }
    else {
        tableWidget->fillTable(words);
    }

    changesSaved = false;
    file.close();
    sendMessage(tr("The file \"%1\" imported").arg(fileName));
}

// To export to a text file in a "|original|translation|status|date\n" format.
void DictionaryWidget::exportToFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QTextStream out(&file);
    QList<WordLine> words = tableWidget->getWords();
    QString oldDateFormat = "dd-MM-yyyy";
    for(auto word : words) {
        word.setDateFormat(oldDateFormat);
        out << QString("|%1|%2|%3|%4\n").arg(word.getOriginal())
                                        .arg(word.getTranslation())
                                        .arg(word.getStatus())
                                        .arg(word.getDate());
    }

    file.close();
    sendMessage(tr("The file \"%1\" exported").arg(fileName));
}

bool DictionaryWidget::isSaved() const
{
    return changesSaved;
}

void DictionaryWidget::setSaved(const bool value)
{
    changesSaved = value;
}

QString DictionaryWidget::getLastFileName() const
{
    return lastFileName;
}

int DictionaryWidget::getRowCount() const
{
    return tableWidget->getRowCount();
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

void DictionaryWidget::clearInput()
{
    inputWidget->clearInput();
}

void DictionaryWidget::createNewFile()
{
    tableWidget->clear();
    lastFileName = "";
    inputWidget->clearInput();
}

void DictionaryWidget::addEntrySlot()
{
    WordLine word = inputWidget->getInput();

    if(word.getOriginal().isEmpty()) {
        //originalLineEdit->setStyleSheet("border: 1px solid red");
        emit sendMessage(tr("Enter the original word"));
        return;
    }

    // User didn't specify the status.
    if(word.getStatus().isEmpty()) {
        word.setStatus(tr("new"));
    }

    word.setCurrentDate();

    QString result;
    if(tableWidget->addEntry(word)) {
        changesSaved = false;
        emit updateMenus();
        //originalLineEdit->setStyleSheet("");
        result = QString(tr("Done"));
    }
    else {
        result = QString(tr("Duplicate word. The word \"%1\" already exists.")
                                                       .arg(word.getOriginal()));
    }

    emit sendMessage(result);
}

void DictionaryWidget::editEntry()
{
    WordLine word = inputWidget->getInput();
    if(word.getOriginal().isEmpty()) {
        //originalLineEdit->setStyleSheet("border: 1px solid red");
        emit sendMessage(tr("Enter the original word"));
        return;
    }

    if(tableWidget->editEntry(word)) {
        changesSaved = false;
        emit updateMenus();
    }
    else {
        emit sendMessage(tr("The word \"%1\" already exists.")
                         .arg(word.getOriginal()));
    }
}

void DictionaryWidget::findEntry()
{
    if(inputWidget->isEmpty()) {
        tableWidget->setFilter(0);
        sendMessage(tr("The filter is cleared"));
        return;
    }

    WordLine word = inputWidget->getInput();
    int columnFind = -1; // Which column will we search for.
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp;
    if(!word.getOriginal().isEmpty()) {
        regExp = QRegExp(word.getOriginal(), Qt::CaseInsensitive, syntax);
        columnFind = 0;
    }
    else if(!word.getTranslation().isEmpty()) {
        regExp = QRegExp(word.getTranslation(), Qt::CaseInsensitive,
                         syntax);
        columnFind = 1;
    }
    else if(!word.getStatus().isEmpty()) {
        regExp = QRegExp(word.getStatus(), Qt::CaseInsensitive, syntax);
        columnFind = 2;
    }
    else if(!word.getDate().isEmpty()) {
        regExp = QRegExp(word.getDate(), Qt::CaseInsensitive, syntax);
        columnFind = 3;
    }
    tableWidget->setFilter(columnFind, regExp);

    QString column = tableWidget->getColumnName(columnFind);
    sendMessage(tr("The filter \"%1\" (%2) is set").arg(regExp.pattern())
                                                   .arg(column));
}

void DictionaryWidget::removeEntry()
{
    tableWidget->removeEntry();
    changesSaved = false;
    emit updateMenus();
}

void DictionaryWidget::updateActions()
{
    WordLine word = tableWidget->getSelectedWord();
    if(!word.isEmpty()) {
        inputWidget->setInput(word);
        inputWidget->setEnabled(InputWidget::DeleteButton, true);
        inputWidget->setEnabled(InputWidget::EditButton, true);
    }
    else { // Dictionary is empty or the row don't selected.
        inputWidget->clearInput();
        inputWidget->setEnabled(InputWidget::DeleteButton, false);
        inputWidget->setEnabled(InputWidget::EditButton, false);
    }
}

DictionaryWidget::DictionaryWidget() : dictionarySettings("Vilcrow", "podiceps")
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    readSettings();

    tableWidget = new TableWidget();
    tableWidget->connectSignals(this);
    mainLayout->addWidget(tableWidget->getTableView());

    inputWidget = new InputWidget();
    inputWidget->connectSignals(this);
    mainLayout->addWidget(inputWidget);

    if(!lastFileName.isEmpty()) {
        readFromFile(lastFileName);
    }

    changesSaved = true;
}

DictionaryWidget::~DictionaryWidget()
{
    writeSettings();
}
