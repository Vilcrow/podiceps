/**************************************************************************/
/*  find_widget.h                                                         */
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

#ifndef FIND_WIDGET_VIL_H
#define FIND_WIDGET_VIL_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QRadioButton;

class FindWidget : public QWidget {
    Q_OBJECT
public:
    void setFocus();
    QString getFilter() const;
    void clearFilter();
    int getChecked() const;


    FindWidget(QWidget *parent = nullptr);
    virtual ~FindWidget();
signals:
    void setClicked();
    void clearClicked();
    void closeClicked();
private slots:
    void buttonChanged(bool checked);
private:
    enum Buttons { OriginalButton, TranscriptionButton, TranslationButton,
                   StatusButton, DateButton, CommentButton };

    enum { MaxFilterLength = 100 };

    QLineEdit *filterLineEdit;
    QPushButton *setButton;
    QPushButton *clearButton;
    QPushButton *closeButton;

    QRadioButton *originalButton;
    QRadioButton *transcriptionButton;
    QRadioButton *translationButton;
    QRadioButton *statusButton;
    QRadioButton *dateButton;
    QRadioButton *commentButton;
    int checkedButton;
};

#endif
