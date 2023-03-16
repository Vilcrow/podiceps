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
#include "find_widget.h"
#include "input_widget.h"
#include "table_widget.h"
#include "word_line.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>

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

bool DictionaryWidget::hasSelectedWords() const
{
    return tableWidget->hasSelectedWords();
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

void DictionaryWidget::addWord(const WordLine& word)
{
    tableWidget->openWordAdd(word);
}

void DictionaryWidget::editWord()
{
    tableWidget->editEntry();
}

void DictionaryWidget::deleteWord()
{
    tableWidget->deleteEntry();
}

void DictionaryWidget::createNewFile()
{
    tableWidget->clear();
    inputWidget->clearInput();
}

void DictionaryWidget::clearInput()
{
    tableWidget->clearSelection();
    if(!inputWidget->isHidden()) {
        inputWidget->clearInput();
    }
    if(!findWidget->isHidden()) {
        findWidget->clearFilter();
    }
}

void DictionaryWidget::setFilter()
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp = QRegExp(findWidget->getFilter(), Qt::CaseInsensitive, syntax);
    int column = findWidget->getChecked(); // Which column will we search for.
    tableWidget->setFilter(column, regExp);
}

void DictionaryWidget::clearFilter()
{
    tableWidget->setFilter();
}

void DictionaryWidget::openFindWidget()
{
    inputWidget->hide();
    findWidget->show();
    findWidget->setFocus();
}

void DictionaryWidget::closeFindWidget()
{
    findWidget->hide();
    inputWidget->show();
    inputWidget->setFocus();
}

void DictionaryWidget::updateInput()
{
    if(tableWidget->hasSelectedWords()) {
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

void DictionaryWidget::resize(int w, int h)
{
    tableWidget->resize(w, h);
}

DictionaryWidget::DictionaryWidget(QWidget *parent)
    : QWidget(parent), settings("Vilcrow", "podiceps")
{
    readSettings();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tableWidget = new TableWidget(this);

    mainLayout->addWidget(tableWidget->getTableView());

    findWidget = new FindWidget(this);
    findWidget->hide();

    mainLayout->addWidget(findWidget);

    inputWidget = new InputWidget(this);
    mainLayout->addWidget(inputWidget);

    connect(tableWidget, &TableWidget::actionCompleted,
            this, &DictionaryWidget::actionCompleted);
    connect(tableWidget, &TableWidget::dataChanged,
            this, &DictionaryWidget::updateInput);
    connect(tableWidget, &TableWidget::dataChanged,
            this, &DictionaryWidget::stateChanged);
    connect(tableWidget, &TableWidget::selectionChanged,
            this, &DictionaryWidget::updateInput);
    connect(tableWidget, &TableWidget::selectionChanged,
            this, &DictionaryWidget::stateChanged);
    connect(this, &DictionaryWidget::undoRequested,
            tableWidget, &TableWidget::undo);
    connect(this, &DictionaryWidget::redoRequested,
            tableWidget, &TableWidget::redo);

    connect(findWidget, &FindWidget::setClicked,
            this, &DictionaryWidget::setFilter);
    connect(findWidget, &FindWidget::clearClicked,
            this, &DictionaryWidget::clearFilter);
    connect(findWidget, &FindWidget::closeClicked,
            this, &DictionaryWidget::closeFindWidget);

    connect(inputWidget, &InputWidget::addWordRequested,
            this, &DictionaryWidget::addWordRequested);
    connect(inputWidget, &InputWidget::editClicked,
            this, &DictionaryWidget::editWordRequested);
    connect(inputWidget, &InputWidget::findClicked,
            this, &DictionaryWidget::openFindWidget);
    connect(inputWidget, &InputWidget::deleteClicked,
            this, &DictionaryWidget::deleteWordRequested);

    connect(this, SIGNAL(addWordRequested(const WordLine&)),
            tableWidget, SLOT(addWord(const WordLine&)));
    connect(this, &DictionaryWidget::editWordRequested,
            tableWidget, &TableWidget::editEntry);
    connect(this, &DictionaryWidget::deleteWordRequested,
            tableWidget, &TableWidget::deleteEntry);

    if(!lastFileName.isEmpty()) {
        readFromFile(lastFileName);
    }

    inputWidget->setFocus();
}

DictionaryWidget::~DictionaryWidget()
{

}
