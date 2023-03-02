/**************************************************************************/
/*  input_widget.cpp                                                      */
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

#include "input_widget.h"
#include "dictionary_widget.h"
#include "word_line.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

WordLine InputWidget::getInput() const
{
    return WordLine(originalLineEdit->text(), translationLineEdit->text(),
                    statusLineEdit->text(), dateLineEdit->text());
}

QString InputWidget::getInput(int index) const
{
    QString ret = QString();

    if(index < OriginalLine || index > AllLines) {
        QString msg = tr("Index is out of bounds: at %1, %2").arg(__FILE__).arg(__LINE__);
        qDebug() << msg;
        return ret;
    }

    switch(index) {
    case OriginalLine:
        ret = originalLineEdit->text();
        break;
    case TranslationLine:
        ret = translationLineEdit->text();
        break;
    case StatusLine:
        ret = statusLineEdit->text();
        break;
    case DateLine:
        ret = dateLineEdit->text();
        break;
    case AllLines:
        break;
    }

    return ret;
}

void InputWidget::setInput(int index, const QString &value)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setText(value);
        break;
    case TranslationLine:
        translationLineEdit->setText(value);
        break;
    case StatusLine:
        statusLineEdit->setText(value);
        break;
    case DateLine:
        dateLineEdit->setText(value);
        break;
    case AllLines:
        break;
    }
}

void InputWidget::setInput(const WordLine &word)
{
    originalLineEdit->setText(word.getOriginal());
    translationLineEdit->setText(word.getTranslation());
    statusLineEdit->setText(word.getStatus());
    dateLineEdit->setText(word.getDate());
}

void InputWidget::setEnabled(int index, bool value)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setEnabled(value);
        break;
    case TranslationLine:
        translationLineEdit->setEnabled(value);
        break;
    case StatusLine:
        statusLineEdit->setEnabled(value);
        break;
    case DateLine:
        dateLineEdit->setEnabled(value);
        break;
    case AllLines:
        originalLineEdit->setEnabled(value);
        translationLineEdit->setEnabled(value);
        statusLineEdit->setEnabled(value);
        dateLineEdit->setEnabled(value);
        break;
    case AddButton:
        addButton->setEnabled(value);
        break;
    case EditButton:
        editButton->setEnabled(value);
        break;
    case FindButton:
        findButton->setEnabled(value);
        break;
    case DeleteButton:
        deleteButton->setEnabled(value);
        break;
    case AllButtons:
        addButton->setEnabled(value);
        editButton->setEnabled(value);
        findButton->setEnabled(value);
        deleteButton->setEnabled(value);
        break;
    }
}

void InputWidget::clearInput(int index)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setText("");
        originalLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case TranslationLine:
        translationLineEdit->setText("");
        translationLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case StatusLine:
        statusLineEdit->setText("");
        statusLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case DateLine:
        dateLineEdit->setText("");
        dateLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case AllLines:
        originalLineEdit->setText("");
        translationLineEdit->setText("");
        statusLineEdit->setText("");
        dateLineEdit->setText("");
        originalLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    }
}

bool InputWidget::isEmpty(int index) const
{
    bool ret = true;

    bool original = originalLineEdit->text().isEmpty();
    bool translation = translationLineEdit->text().isEmpty();
    bool status = statusLineEdit->text().isEmpty();
    bool date = dateLineEdit->text().isEmpty();

    switch(index) {
    case OriginalLine:
        ret = original;
        break;
    case TranslationLine:
        ret = translation;
        break;
    case StatusLine:
        ret = status;
        break;
    case DateLine:
        ret = date;
        break;
    case AllLines:
        ret = original && translation && status && date;
        break;
    }

    return ret;
}

void InputWidget::connectSignals(DictionaryWidget *dictWidget)
{
    if(dictWidget) {
        connect(addButton, &QAbstractButton::clicked,
                dictWidget, &DictionaryWidget::addEntrySlot);
        connect(editButton, &QAbstractButton::clicked,
                dictWidget, &DictionaryWidget::editEntry);
        connect(findButton, &QAbstractButton::clicked,
                dictWidget, &DictionaryWidget::findEntry);
        connect(deleteButton, &QAbstractButton::clicked,
                dictWidget, &DictionaryWidget::removeEntry);
    }
}

InputWidget::InputWidget()
{
    mainLayout = new QVBoxLayout(this);

    // Labels.
    originalLabel = new QLabel(tr("Original"));
    translationLabel = new QLabel(tr("Translation"));
    statusLabel = new QLabel(tr("Status"));
    dateLabel = new QLabel(tr("Date"));

    // Edit lines.
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

    // Buttons.
    buttonsLayout = new QHBoxLayout();

    addButton = new QPushButton(tr("Add"));
    buttonsLayout->addWidget(addButton);

    editButton = new QPushButton(tr("Edit"));
    editButton->setEnabled(false);
    buttonsLayout->addWidget(editButton);

    findButton = new QPushButton(tr("Find"));
    buttonsLayout->addWidget(findButton);

    deleteButton = new QPushButton(tr("Delete"));
    deleteButton->setEnabled(false);
    buttonsLayout->addWidget(deleteButton);

    mainLayout->addLayout(buttonsLayout);
}

InputWidget::~InputWidget()
{

}
