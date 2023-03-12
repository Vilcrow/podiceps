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
#include "word_line.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

WordLine InputWidget::getInput() const
{
    QString original, transcription, translation, status;
    original = originalLineEdit->text();
    transcription = transcriptionLineEdit->text();
    translation = translationLineEdit->text();
    status = statusLineEdit->text();

    WordLine word(original, translation, status);
    word.setTranscription(transcription);

    return word;
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
    case TranscriptionLine:
        ret = transcriptionLineEdit->text();
        break;
    case TranslationLine:
        ret = translationLineEdit->text();
        break;
    case StatusLine:
        ret = statusLineEdit->text();
        break;
    case AllLines:
        break;
    }

    return ret;
}

void InputWidget::setInput(const WordLine &word)
{
    originalLineEdit->setText(word.getOriginal());
    transcriptionLineEdit->setText(word.getTranscription());
    translationLineEdit->setText(word.getTranslation());
    statusLineEdit->setText(word.getStatus());
}

void InputWidget::setInput(int index, const QString &value)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setText(value);
        break;
    case TranscriptionLine:
        transcriptionLineEdit->setText(value);
        break;
    case TranslationLine:
        translationLineEdit->setText(value);
        break;
    case StatusLine:
        statusLineEdit->setText(value);
        break;
    case AllLines:
        break;
    }
}

void InputWidget::setEnabled(int index, bool value)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setEnabled(value);
        break;
    case TranscriptionLine:
        transcriptionLineEdit->setEnabled(value);
        break;
    case TranslationLine:
        translationLineEdit->setEnabled(value);
        break;
    case StatusLine:
        statusLineEdit->setEnabled(value);
        break;
    case AllLines:
        originalLineEdit->setEnabled(value);
        transcriptionLineEdit->setEnabled(value);
        translationLineEdit->setEnabled(value);
        statusLineEdit->setEnabled(value);
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

void InputWidget::setFocus()
{
    originalLineEdit->setFocus(Qt::OtherFocusReason);
}

void InputWidget::clearInput(int index)
{
    switch(index) {
    case OriginalLine:
        originalLineEdit->setText("");
        originalLineEdit->setStyleSheet("");
        originalLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case TranscriptionLine:
        transcriptionLineEdit->setText("");
        transcriptionLineEdit->setStyleSheet("");
        transcriptionLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case TranslationLine:
        translationLineEdit->setText("");
        translationLineEdit->setStyleSheet("");
        translationLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case StatusLine:
        statusLineEdit->setText("");
        statusLineEdit->setStyleSheet("");
        statusLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    case AllLines:
        originalLineEdit->setText("");
        originalLineEdit->setStyleSheet("");
        transcriptionLineEdit->setText("");
        transcriptionLineEdit->setStyleSheet("");
        translationLineEdit->setText("");
        translationLineEdit->setStyleSheet("");
        statusLineEdit->setText("");
        statusLineEdit->setStyleSheet("");
        originalLineEdit->setFocus(Qt::OtherFocusReason);
        break;
    }
}

bool InputWidget::isEmpty(int index) const
{
    bool ret = true;

    bool original = originalLineEdit->text().isEmpty();
    bool transcription = transcriptionLineEdit->text().isEmpty();
    bool translation = translationLineEdit->text().isEmpty();
    bool status = statusLineEdit->text().isEmpty();

    switch(index) {
    case OriginalLine:
        ret = original;
        break;
    case TranscriptionLine:
        ret = transcription;
        break;
    case TranslationLine:
        ret = translation;
        break;
    case StatusLine:
        ret = status;
        break;
    case AllLines:
        ret = original && transcription && translation && status;
        break;
    }

    return ret;
}

void InputWidget::addWord()
{
    WordLine word = getInput();
    word.setCurrentDate();
    clearInput();
    emit addWordRequested(word);
}

InputWidget::InputWidget(QWidget *parent)
    : QWidget(parent), originalLineEdit(new QLineEdit),
      transcriptionLineEdit(new QLineEdit), translationLineEdit(new QLineEdit),
      statusLineEdit(new QLineEdit)
{
    originalLineEdit->setPlaceholderText(tr("Original..."));
    originalLineEdit->setMaxLength(WordLine::MaxOriginalLength);

    transcriptionLineEdit->setPlaceholderText(tr("Transcription..."));
    transcriptionLineEdit->setMaxLength(WordLine::MaxOriginalLength);

    translationLineEdit->setPlaceholderText(tr("Translation..."));
    translationLineEdit->setMaxLength(WordLine::MaxTranslationLength);

    statusLineEdit->setPlaceholderText(tr("Status..."));
    statusLineEdit->setMaxLength(WordLine::MaxStatusLength);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(originalLineEdit, 0, 0);
    mainLayout->addWidget(transcriptionLineEdit, 0, 1);
    mainLayout->addWidget(translationLineEdit, 0, 2);
    mainLayout->addWidget(statusLineEdit, 0, 3);

    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("Ed&it"));
    editButton->setEnabled(false);
    deleteButton = new QPushButton(tr("&Delete"));
    deleteButton->setEnabled(false);
    findButton = new QPushButton(tr("Fi&nd"));

    mainLayout->addWidget(addButton, 1, 0);
    mainLayout->addWidget(editButton, 1, 1);
    mainLayout->addWidget(deleteButton, 1, 2);
    mainLayout->addWidget(findButton, 1, 3);

    setLayout(mainLayout);

    connect(originalLineEdit, &QLineEdit::returnPressed,
            this, &InputWidget::addWord);
    connect(transcriptionLineEdit, &QLineEdit::returnPressed,
            this, &InputWidget::addWord);
    connect(translationLineEdit, &QLineEdit::returnPressed,
            this, &InputWidget::addWord);
    connect(statusLineEdit, &QLineEdit::returnPressed,
            this, &InputWidget::addWord);

    connect(addButton, &QAbstractButton::clicked,
            this, &InputWidget::addWord);
    connect(editButton, &QAbstractButton::clicked,
            this, &InputWidget::editClicked);
    connect(deleteButton, &QAbstractButton::clicked,
            this, &InputWidget::deleteClicked);
    connect(findButton, &QAbstractButton::clicked,
            this, &InputWidget::findClicked);
}

InputWidget::~InputWidget()
{

}
