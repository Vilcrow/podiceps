/**************************************************************************/
/*  status_spin_box.cpp                                                   */
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

#include "status_spin_box.h"
#include "word_status.h"
#include <QKeyEvent>
#include <QLineEdit>

QString StatusSpinBox::getValue() const
{
    return textFromValue(value());
}

void StatusSpinBox::setValue(const QString &value)
{
    QSpinBox::setValue(valueFromText(value));
}

QString StatusSpinBox::textFromValue(int value) const
{
    WordStatus status;
    status.setStatus(value);
    return status.getStatus();
}

int StatusSpinBox::valueFromText(const QString &text) const
{
    WordStatus status(text);
    return status.getStatusInt();
}

void StatusSpinBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return) {
        emit returnPressed();
    }
}

void StatusSpinBox::changeColor(int i)
{
    WordStatus status;
    status = i;
    QColor color = WordStatus::getColor(status.getStatusInt());
    QString name = color.name();
    this->setStyleSheet("background-color: " + name);
}

StatusSpinBox::StatusSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setRange(WordStatus::New, WordStatus::Learned);
    changeColor(0);
    lineEdit()->setReadOnly(true);

    connect(this, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &StatusSpinBox::changeColor);
}

StatusSpinBox::~StatusSpinBox()
{

}
