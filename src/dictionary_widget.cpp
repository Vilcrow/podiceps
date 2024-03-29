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
#include "settings.h"
#include "table_widget.h"
#include "word_line.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
// For converting QTableView* to QWidget* in the constructor
#include <QTableView>
#include <QTextStream>
#include <QVBoxLayout>

void DictionaryWidget::readFromFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        qDebug() << "Passed an empty argument";
        return;
    }

    QFile xmlFile(fileName);
    if(!xmlFile.open(QIODevice::ReadOnly)) {
        // The last opened file isn't available.
        if(fileName == lastFileName) {
            lastFileName = "";
            emit stateChanged();
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
        emit stateChanged();
    }
}

void DictionaryWidget::writeToFile(const QString &fileName)
{
    if(fileName.isEmpty()) {
        qDebug() << "Passed an empty argument";
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
    QFile xmlFile(fileName);
    if(!xmlFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 xmlFile.errorString());
        return false;
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

    return true;
}

void DictionaryWidget::setLastFileName(const QString &newLast)
{
    lastFileName = newLast;
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
    lastFileName = Settings::getLastFileName();
}

void DictionaryWidget::writeSettings()
{
    Settings::setLastFileName(lastFileName);
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
    setLastFileName("");
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
    int column = findWidget->getChecked(); // Which column will we search for.
    QString filter = findWidget->getFilter();
    tableWidget->setFilter(column, filter);
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
    }
    else { // Dictionary is empty or the row don't selected.
        inputWidget->clearInput();
        inputWidget->setEnabled(InputWidget::DeleteButton, false);
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
    : QWidget(parent)
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
    connect(this, &DictionaryWidget::statisticsRequested,
            tableWidget, &TableWidget::openStatistics);

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
