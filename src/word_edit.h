/**************************************************************************/
/*  word_edit.h                                                           */
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

#ifndef WORD_EDIT_VIL_H
#define WORD_EDIT_VIL_H

#include "word_line.h"
#include <QDialog>

class StatusSpinBox;
class QLabel;
class QLineEdit;
class QStatusBar;
class QTextEdit;

class WordEdit : public QDialog {
    Q_OBJECT
public:
    QList<WordLine> getWords() const;

    WordEdit(const QList<WordLine> &pWords,
             const QList<int> &pPositions, QWidget *parent = nullptr);
    virtual ~WordEdit();
public slots:
    void accept() override;
private slots:
    void truncateComment();
    void saveWord();
    void deleteWord();
    void recoverWord();
    void stepPrev();
    void stepNext();
private:
    QList<WordLine> words;
    QList<int> positions;
    int position;
    int posIndex;

    QMap<int, WordLine> sourceValues;
    QList<int> deletedWords;

    QLabel *originalLabel;
    QLabel *transcriptionLabel;
    QLabel *translationLabel;
    QLabel *statusLabel;
    QLabel *dateLabel;
    QLabel *commentLabel;

    QPushButton *saveButton;
    QPushButton *deleteButton;
    QPushButton *recoverButton;
    QPushButton *prevButton;
    QPushButton *nextButton;

    QLineEdit *originalText;
    QLineEdit *transcriptionText;
    QLineEdit *translationText;
    StatusSpinBox *statusSpinBox;
    QLineEdit *dateText;
    QTextEdit *commentText;
    QStatusBar *statusBar;

    void update();
    void setWord();
    WordLine getWord() const;
    void setEditable(bool value);
    void showMessage(const QString &msg);
    void fillFields(const WordLine &word);
};

#endif
