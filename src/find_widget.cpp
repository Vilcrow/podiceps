/**************************************************************************/
/*  find_widget.cpp                                                       */
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

#include "find_widget.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

QString FindWidget::getFilter() const
{
    return filterLineEdit->text();
}

int FindWidget::getChecked() const
{
    return checkedButton;
}

void FindWidget::clearFilter()
{
    filterLineEdit->clear();
    setFocus();
}

void FindWidget::setFocus()
{
    filterLineEdit->setFocus(Qt::OtherFocusReason);
}

void FindWidget::buttonChanged(bool checked)
{
    if(checked) {
        QRadioButton *button = static_cast<QRadioButton*>(sender());
        if(button == originalButton) {
            checkedButton = OriginalButton;
        }
        else if(button == transcriptionButton) {
            checkedButton = TranscriptionButton;
        }
        else if(button == translationButton) {
            checkedButton = TranslationButton;
        }
        else if(button == statusButton) {
            checkedButton = StatusButton;
        }
        else if(button == dateButton) {
            checkedButton = DateButton;
        }
        else if(button == commentButton) {
            checkedButton = CommentButton;
        }
    }
}

FindWidget::FindWidget(QWidget *parent)
    : QWidget(parent), filterLineEdit(new QLineEdit)
{
    filterLineEdit->setPlaceholderText(tr("Enter a filter to search for..."));
    filterLineEdit->setMaxLength(MaxFilterLength);

    setButton = new QPushButton(tr("Set"));
    clearButton = new QPushButton(tr("Clear"));
    closeButton = new QPushButton(tr("Close"));
    closeButton->setShortcut(Qt::ALT + Qt::Key_F);

    QHBoxLayout *filterLayout = new QHBoxLayout;

    filterLayout->addWidget(filterLineEdit);
    filterLayout->addWidget(setButton);
    filterLayout->addWidget(clearButton);
    filterLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout(filterLayout);

    QGroupBox *radioButtonsGroupBox = new QGroupBox;
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout;

    originalButton = new QRadioButton(tr("&original"));
    originalButton->setChecked(true);
    checkedButton = OriginalButton;
    transcriptionButton = new QRadioButton(tr("t&ranscription"));
    translationButton = new QRadioButton(tr("&translation"));
    statusButton = new QRadioButton(tr("&status"));
    dateButton = new QRadioButton(tr("&date"));
    commentButton = new QRadioButton(tr("&comment"));

    radioButtonsLayout->addWidget(originalButton);
    radioButtonsLayout->addWidget(transcriptionButton);
    radioButtonsLayout->addWidget(translationButton);
    radioButtonsLayout->addWidget(statusButton);
    radioButtonsLayout->addWidget(dateButton);
    radioButtonsLayout->addWidget(commentButton);
    radioButtonsGroupBox->setLayout(radioButtonsLayout);

    mainLayout->addWidget(radioButtonsGroupBox);
    setLayout(mainLayout);

    connect(filterLineEdit, &QLineEdit::returnPressed,
            this, &FindWidget::setClicked);
    connect(this, &FindWidget::clearClicked, this, &FindWidget::clearFilter);

    connect(setButton, &QAbstractButton::clicked,
            this, &FindWidget::setClicked);
    connect(clearButton, &QAbstractButton::clicked,
            this, &FindWidget::clearClicked);
    connect(closeButton, &QAbstractButton::clicked,
            this, &FindWidget::closeClicked);

    connect(originalButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
    connect(transcriptionButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
    connect(translationButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
    connect(statusButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
    connect(dateButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
    connect(commentButton, &QAbstractButton::toggled,
            this, &FindWidget::buttonChanged);
}

FindWidget::~FindWidget()
{

}
