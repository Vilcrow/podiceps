/**************************************************************************/
/*  input_widget.h                                                        */
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

#ifndef INPUT_WIDGET_VIL_H
#define INPUT_WIDGET_VIL_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

class WordLine;
class DictionaryWidget;
class QLabel;
class QGridLayout;
class QPushButton;
class QLineEdit;

class InputWidget : public QWidget {
    Q_OBJECT
public:
    enum Lines { OriginalLine, TranslationLine,
                 StatusLine, DateLine, AllLines };

    enum Buttons { AddButton = AllLines + 1, EditButton,
                   FindButton, DeleteButton, AllButtons };

    WordLine getInput() const;
    QString getInput(int index) const;

    void setInput(const WordLine &word);
    void setInput(int index, const QString &value);
    void setEnabled(int index, bool value = true);

    void clearInput(int index = AllLines);
    bool isEmpty(int index = AllLines) const;

    void connectSignals(DictionaryWidget *dictWidget);

    InputWidget(QWidget *parent = nullptr);
    virtual ~InputWidget();
private:
    QLabel *originalLabel;
    QLabel *translationLabel;
    QLabel *statusLabel;
    QLabel *dateLabel;

    QLineEdit *originalLineEdit;
    QLineEdit *translationLineEdit;
    QLineEdit *statusLineEdit;
    QLineEdit *dateLineEdit;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *findButton;
    QPushButton *deleteButton;

    QVBoxLayout *mainLayout;
    QGridLayout *inputLayout;
    QHBoxLayout *buttonsLayout;
};

#endif
