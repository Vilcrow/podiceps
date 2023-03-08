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
#include "input_widget.h"
#include "table_widget.h"
#include "word_line.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

void DictionaryWidget::readFromFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    QFile xmlFile(fileName);
    if(!xmlFile.open(QIODevice::ReadOnly)) {
        if(fileName == lastFileName) {
            lastFileName = "";
        }
        actionCompleted(tr("Unable to open file: %1").arg(fileName));
        return;
    }

    QDomDocument document;
    document.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = document.documentElement();
    QDomElement node = root.firstChild().toElement();

    QList<WordLine> words;
    while(!node.isNull()) {
        if(node.tagName() == "word") {
            while(!node.isNull()) {
                WordLine word(node);
                words.append(word);
                node = node.nextSibling().toElement();
            }
        }
        node = node.nextSibling().toElement();
    }

    if(words.isEmpty()) {
        actionCompleted(tr("The file contains no words or "
                           "has an invalid format."));
    }
    else {
        tableWidget->fillTable(words);
        lastFileName = fileName;
        setSaved(true);
        actionCompleted(tr("The file \"%1\" is open").arg(lastFileName));
    }
}

void DictionaryWidget::writeToFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }

    if(writeToXmlFile(fileName)) {
        lastFileName = fileName;
        setSaved(true);
        actionCompleted(tr("The file \"%1\" saved").arg(lastFileName));
    }
}

bool DictionaryWidget::writeToXmlFile(const QString &fileName)
{
    bool success = false;

    QFile xmlFile(fileName);
    if(!xmlFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 xmlFile.errorString());
        success = false;
        return success;
    };

    QTextStream xmlContent(&xmlFile);
    QDomDocument document;
    QDomElement root = document.createElement("Words");
    document.appendChild(root);

    QList<WordLine> words = tableWidget->getWords();
    for(WordLine word : words) {
        QDomElement domElement = document.createElement("word");
        word.setDomElement(domElement);
        root.appendChild(domElement);
    }

    xmlContent << document.toString();
    xmlFile.close();
    success = true;

    return success;
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
            tr("The file contains no words."));
    }
    else {
        tableWidget->fillTable(words);
        setSaved(false);
    }

    file.close();
    actionCompleted(tr("The file \"%1\" imported").arg(fileName));
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
    actionCompleted(tr("The file \"%1\" exported").arg(fileName));
}

bool DictionaryWidget::isSaved() const
{
    return tableWidget->isSaved();
}

void DictionaryWidget::setSaved(bool value)
{
    tableWidget->setSaved(value);
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
    settings.beginGroup("/Settings");
    lastFileName = settings.value("/last_file_name", "").toString();
    settings.endGroup();
}

void DictionaryWidget::writeSettings()
{
    settings.beginGroup("/Settings");
    settings.setValue("last_file_name", lastFileName);
    settings.endGroup();
}

void DictionaryWidget::clearInput()
{
    inputWidget->clearInput();
    tableWidget->clearSelection();
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
        inputWidget->setStyleSheet(InputWidget::OriginalLine,
                                   "border: 1px solid red");
        emit actionCompleted(tr("Enter the original word"));
        return;
    }

    // User didn't specify the status.
    if(word.getStatus().isEmpty()) {
        word.setStatus(tr("new"));
    }

    word.setCurrentDate();

    QString result;
    if(tableWidget->addEntry(word)) {
        emit stateChanged();
        inputWidget->setStyleSheet(InputWidget::OriginalLine, "");
        result = QString(tr("Added"));
    }
    else {
        result = QString(tr("Duplicate word. The word \"%1\" already exists.")
                                                       .arg(word.getOriginal()));
    }

    emit actionCompleted(result);
}

void DictionaryWidget::editEntry()
{
    WordLine word = inputWidget->getInput();
    if(word.getOriginal().isEmpty()) {
        inputWidget->setStyleSheet(InputWidget::OriginalLine,
                                   "border: 1px solid red");
        emit actionCompleted(tr("Enter the original word"));
        return;
    }

    if(tableWidget->editEntry(word)) {
        emit stateChanged();
        emit actionCompleted(tr("Edited"));
    }
    else {
        emit actionCompleted(tr("The word \"%1\" already exists.")
                         .arg(word.getOriginal()));
    }
}

void DictionaryWidget::findEntry()
{
    if(inputWidget->isEmpty()) {
        tableWidget->setFilter(0);
        actionCompleted(tr("The filter is cleared"));
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
    actionCompleted(tr("The filter \"%1\" (%2) is set").arg(regExp.pattern())
                                                   .arg(column));
}

void DictionaryWidget::removeEntry()
{
    tableWidget->removeEntry();
    emit stateChanged();
    actionCompleted(tr("Deleted"));
}

void DictionaryWidget::updateInput()
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

void DictionaryWidget::updateSettings()
{
    tableWidget->updateSettings();
}

DictionaryWidget::DictionaryWidget(QWidget *parent)
    : QWidget(parent), settings("Vilcrow", "podiceps")
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    readSettings();

    tableWidget = new TableWidget(this);
    connect(tableWidget, &TableWidget::dataChanged,
            this, &DictionaryWidget::updateInput);
    connect(tableWidget, &TableWidget::dataChanged,
            this, &DictionaryWidget::stateChanged);
    connect(tableWidget, &TableWidget::selectionChanged,
            this, &DictionaryWidget::updateInput);

    mainLayout->addWidget(tableWidget->getTableView());

    inputWidget = new InputWidget(this);
    connect(inputWidget, &InputWidget::addClicked,
            this, &DictionaryWidget::addEntrySlot);
    connect(inputWidget, &InputWidget::editClicked,
            this, &DictionaryWidget::editEntry);
    connect(inputWidget, &InputWidget::findClicked,
            this, &DictionaryWidget::findEntry);
    connect(inputWidget, &InputWidget::deleteClicked,
            this, &DictionaryWidget::removeEntry);
    mainLayout->addWidget(inputWidget);

    if(!lastFileName.isEmpty()) {
        readFromFile(lastFileName);
    }

}

DictionaryWidget::~DictionaryWidget()
{

}
