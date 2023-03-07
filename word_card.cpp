/**************************************************************************/
/*  word_card.cpp                                                         */
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

#include "word_card.h"
#include "word_line.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

void WordCard::accept()
{
    word.setOriginal(originalText->text());
    word.setTranslation(translationText->text());
    word.setStatus(statusText->text());
    word.setComment(commentText->toPlainText());
    QDialog::accept();
}

WordLine WordCard::getWord() const
{
    return word;
}

void WordCard::truncateComment()
{
    if(commentText->toPlainText().length() > WordLine::MaxCommentLength) {
        QTextCursor cursor = commentText->textCursor();
        commentText->setTextCursor(cursor);
        cursor.deletePreviousChar();
    }
}

WordCard::WordCard(const WordLine &pWord, QWidget *parent)
    : QDialog(parent), word(pWord), originalText(new QLineEdit),
      translationText(new QLineEdit), statusText(new QLineEdit),
      dateText(new QLineEdit), commentText(new QTextEdit)
{
    originalText->setMaxLength(WordLine::MaxOriginalLength);
    translationText->setMaxLength(WordLine::MaxTranslationLength);
    statusText->setMaxLength(WordLine::MaxStatusLength);
    dateText->setMaxLength(WordLine::MaxDateLength);

    originalText->setText(word.getOriginal());
    translationText->setText(word.getTranslation());
    statusText->setText(word.getStatus());
    dateText->setText(word.getDate());
    dateText->setEnabled(false);
    commentText->setText(word.getComment());

    QGridLayout *mainLayout = new QGridLayout;

    QLabel *originalLabel = new QLabel(tr("Original"));
    QLabel *translationLabel = new QLabel(tr("Translation"));
    QLabel *statusLabel = new QLabel(tr("Status"));
    QLabel *dateLabel = new QLabel(tr("Date"));
    QLabel *commentLabel = new QLabel(tr("Comment"));

    mainLayout->addWidget(originalLabel, 0, 0);
    mainLayout->addWidget(originalText, 0, 1);
    mainLayout->addWidget(translationLabel, 1, 0);
    mainLayout->addWidget(translationText, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0);
    mainLayout->addWidget(statusText, 2, 1);
    mainLayout->addWidget(dateLabel, 3, 0);
    mainLayout->addWidget(dateText, 3, 1);
    mainLayout->addWidget(commentLabel, 4, 0, Qt::AlignTop);
    mainLayout->addWidget(commentText, 4, 1);

    connect(commentText, &QTextEdit::textChanged,
            this, &WordCard::truncateComment);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *saveButton = new QPushButton(tr("&Save"));
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout, 5, 1, Qt::AlignRight);

    setLayout(mainLayout);
    setWindowTitle(tr("Edit a Word"));

    connect(saveButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}

WordCard::~WordCard()
{

}
