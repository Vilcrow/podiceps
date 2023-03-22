/**************************************************************************/
/*  word_edit.cpp                                                         */
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

#include "word_edit.h"
#include "status_spin_box.h"
#include "word_line.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <algorithm>

QList<WordLine> WordEdit::getWords() const
{
    return words;
}

void WordEdit::accept()
{
    sourceValues.clear();

    if(!deletedWords.isEmpty()) {
        std::sort(deletedWords.begin(), deletedWords.end());
        std::reverse(deletedWords.begin(), deletedWords.end());
        for(int i = 0; i < deletedWords.size(); ++i) {
            int pos = deletedWords[i];
            words.removeAt(pos);
        }
        deletedWords.clear();
    }

    QDialog::accept();
}

void WordEdit::truncateComment()
{
    if(commentText->toPlainText().length() > WordLine::MaxCommentLength) {
        QTextCursor cursor = commentText->textCursor();
        commentText->setTextCursor(cursor);
        cursor.deletePreviousChar();
    }
}

void WordEdit::saveWord()
{
    if(getWord() == words[position]) {
        showMessage(tr("No changes"));
        return;
    }

    QString original = originalText->text();
    if(original.isEmpty()) {
        showMessage(tr("Enter the original word"));
        return;
    }

    if(words[position].getOriginal() != original &&
                words.contains(WordLine(original))) {
        showMessage(tr("The word \"%1\" already exists").arg(original));
        return;
    }

    // Save the source value.
    if(sourceValues.find(position) == sourceValues.end()) {
        sourceValues[position] = words[position];
    }

    setWord();
    update();
    showMessage(tr("Saved"));
}

void WordEdit::deleteWord()
{
    // Save the source value.
    if(sourceValues.find(position) == sourceValues.end()) {
        sourceValues[position] = words[position];
    }

    deletedWords.append(position);
    update();

    showMessage(tr("Deleted"));
}

void WordEdit::recoverWord()
{
    if(sourceValues.find(position) == sourceValues.end()) {
        return;
    }

    WordLine word = sourceValues[position];
    deletedWords.removeAll(position);
    fillFields(word);
    saveWord();
    update();

    showMessage(tr("Recovered"));
}

void WordEdit::stepPrev()
{
    if(position == positions.first()) {
        return;
    }

    position = positions[--posIndex];
    fillFields(words[position]);
    update();
}

void WordEdit::stepNext()
{
    if(position == positions.last()) {
        return;
    }

    position = positions[++posIndex];
    fillFields(words[position]);
    update();
}

void WordEdit::update()
{
    if(deletedWords.contains(position)) {
        setEditable(false);
        deleteButton->setEnabled(false);
    }
    else {
        setEditable(true);
        deleteButton->setEnabled(true);
    }

    if(sourceValues.find(position) == sourceValues.end()) {
        recoverButton->setEnabled(false);
    }
    else {
        recoverButton->setEnabled(true);
    }

    if(position == positions.first()) {
        prevButton->setEnabled(false);
    }
    else {
        prevButton->setEnabled(true);
    }

    if(position == positions.last()) {
        nextButton->setEnabled(false);
    }
    else {
        nextButton->setEnabled(true);
    }
}

void WordEdit::setWord()
{
    words[position].setOriginal(originalText->text());
    words[position].setTranslation(translationText->text());
    words[position].setStatus(statusSpinBox->getValue());
    words[position].setComment(commentText->toPlainText());
    words[position].setTranscription(transcriptionText->text());
}

WordLine WordEdit::getWord() const
{
    WordLine word = words[position];
    word.setOriginal(originalText->text());
    word.setTranslation(translationText->text());
    word.setStatus(statusSpinBox->getValue());
    word.setComment(commentText->toPlainText());
    word.setTranscription(transcriptionText->text());

    return word;
}

// For deleted words.
void WordEdit::setEditable(bool value)
{
    originalLabel->setEnabled(value);
    originalText->setEnabled(value);
    transcriptionLabel->setEnabled(value);
    transcriptionText->setEnabled(value);
    translationLabel->setEnabled(value);
    translationText->setEnabled(value);
    statusLabel->setEnabled(value);
    statusSpinBox->setEnabled(value);
    dateLabel->setEnabled(value);
    commentLabel->setEnabled(value);
    commentText->setEnabled(value);
}

void WordEdit::showMessage(const QString &msg)
{
    statusBar->showMessage(msg, 5000);
}

void WordEdit::fillFields(const WordLine &word)
{
    originalText->setText(word.getOriginal());
    transcriptionText->setText(word.getTranscription());
    translationText->setText(word.getTranslation());
    statusSpinBox->setValue(word.getStatus());
    dateText->setText(word.getDate());
    commentText->setText(word.getComment());
}

WordEdit::WordEdit(const QList<WordLine> &pWords,
                   const QList<int> &pPositions, QWidget *parent)
    : QDialog(parent), words(pWords), positions(pPositions),
      position(positions.first()), posIndex(0),
      sourceValues(QMap<int, WordLine>()), deletedWords(QList<int>())
{
    originalText = new QLineEdit;
    transcriptionText = new QLineEdit;
    translationText = new QLineEdit;
    statusSpinBox = new StatusSpinBox;
    dateText = new QLineEdit;
    commentText = new QTextEdit;
    statusBar = new QStatusBar(this);

    originalText->setMaxLength(WordLine::MaxOriginalLength);
    transcriptionText->setMaxLength(WordLine::MaxOriginalLength);
    translationText->setMaxLength(WordLine::MaxTranslationLength);
    dateText->setMaxLength(WordLine::MaxDateLength);

    originalText->setText(words[position].getOriginal());
    transcriptionText->setText(words[position].getTranscription());
    translationText->setText(words[position].getTranslation());
    statusSpinBox->setValue(words[position].getStatus());
    dateText->setText(words[position].getDate());
    dateText->setEnabled(false);
    commentText->setText(words[position].getComment());

    QGridLayout *gridLayout = new QGridLayout;

    originalLabel = new QLabel(tr("Original"));
    transcriptionLabel = new QLabel(tr("Transcription"));
    translationLabel = new QLabel(tr("Translation"));
    statusLabel = new QLabel(tr("Status"));
    dateLabel = new QLabel(tr("Date"));
    commentLabel = new QLabel(tr("Comment"));

    gridLayout->addWidget(originalLabel, 0, 0);
    gridLayout->addWidget(originalText, 0, 1);
    gridLayout->addWidget(transcriptionLabel, 1, 0);
    gridLayout->addWidget(transcriptionText, 1, 1);
    gridLayout->addWidget(translationLabel, 2, 0);
    gridLayout->addWidget(translationText, 2, 1);
    gridLayout->addWidget(statusLabel, 3, 0);
    gridLayout->addWidget(statusSpinBox, 3, 1);
    gridLayout->addWidget(dateLabel, 4, 0);
    gridLayout->addWidget(dateText, 4, 1);
    gridLayout->addWidget(commentLabel, 5, 0, Qt::AlignTop);
    gridLayout->addWidget(commentText, 5, 1);

    QHBoxLayout *topButtonLayout = new QHBoxLayout;
    saveButton = new QPushButton(tr("&Save"));
    deleteButton = new QPushButton(tr("&Delete"));
    recoverButton = new QPushButton(tr("&Recover"));
    topButtonLayout->addWidget(saveButton);
    topButtonLayout->addWidget(deleteButton);
    topButtonLayout->addWidget(recoverButton);

    QHBoxLayout *lowButtonLayout = new QHBoxLayout;
    prevButton = new QPushButton(tr("&Prev"));
    nextButton = new QPushButton(tr("&Next"));
    lowButtonLayout->addWidget(prevButton);
    lowButtonLayout->addWidget(nextButton);

    QPushButton *quitButton = new QPushButton(tr("&Quit"));
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));

    gridLayout->addLayout(topButtonLayout, 6, 1, Qt::AlignRight);
    gridLayout->addWidget(quitButton, 6, 0, Qt::AlignLeft);
    gridLayout->addWidget(cancelButton, 7, 0, Qt::AlignLeft);
    gridLayout->addLayout(lowButtonLayout, 7, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(statusBar);

    setLayout(mainLayout);
    setWindowTitle(tr("Edit a Word"));
    update();

    connect(saveButton, &QAbstractButton::clicked, this, &WordEdit::saveWord);
    connect(deleteButton, &QAbstractButton::clicked,
            this, &WordEdit::deleteWord);
    connect(recoverButton, &QAbstractButton::clicked,
            this, &WordEdit::recoverWord);

    connect(prevButton, &QAbstractButton::clicked, this, &WordEdit::stepPrev);
    connect(nextButton, &QAbstractButton::clicked, this, &WordEdit::stepNext);
    connect(quitButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    connect(commentText, &QTextEdit::textChanged,
            this, &WordEdit::truncateComment);
}

WordEdit::~WordEdit()
{

}
