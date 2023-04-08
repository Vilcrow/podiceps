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
#include <QLineEdit>  // For the lineEdit() method in the constructor

QString StatusSpinBox::getValue() const
{
    WordStatus status(value());
    return status.getStatus();
}

void StatusSpinBox::setValue(const QString &value)
{
    QSpinBox::setValue(valueFromText(value));
}

// Displays the status in the custom language.
QString StatusSpinBox::textFromValue(int value) const
{
    WordStatus status(value);
    return status.getStatusTranslation();
}

int StatusSpinBox::valueFromText(const QString &text) const
{
    WordStatus status(text);
    return status.getStatusInt();
}

void StatusSpinBox::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch(key) {
    case Qt::Key_Return:
        emit returnPressed();
        break;
    case Qt::Key_W:
    case Qt::Key_K:
        stepUp();
        break;
    case Qt::Key_S:
    case Qt::Key_J:
        stepDown();
        break;
    default:
        break;
    }

    QSpinBox::keyPressEvent(event);
}

void StatusSpinBox::setColor(int status)
{
    WordStatus s(status);
    QColor color = WordStatus::getColor(s.getStatusInt());
    QString name = color.name();
    setStyleSheet("background-color: " + name);
}

StatusSpinBox::StatusSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setRange(WordStatus::New, WordStatus::Learned);
    setColor(0);
    lineEdit()->setReadOnly(true);
    setContextMenuPolicy(Qt::NoContextMenu);

    connect(this, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &StatusSpinBox::setColor);
}

StatusSpinBox::~StatusSpinBox()
{

}
