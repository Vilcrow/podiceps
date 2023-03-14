/**************************************************************************/
/*  word_status.cpp                                                       */
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

#include "word_status.h"
#include <QObject>

QMap<int, QColor> WordStatus::map = { { New, QColor(51, 51, 255) },
                                      { Middle, QColor(204, 102, 0) },
                                      { Learned, QColor(0, 153, 0) } };

QColor WordStatus::getColor(int i)
{
    if(i < New || i > Learned) {
        i = New;
    }

    return map[i];
}

void WordStatus::setStatus(int i)
{
    switch(i) {
    case New:
        status = New;
        break;
    case Middle:
        status = Middle;
        break;
    case Learned:
        status = Learned;
        break;
    default:
        status = New;
        break;
    }
}

void WordStatus::setStatus(const QString s)
{
    if(s == QObject::tr("middle")) {
        status = Middle;
    }
    else if(s == QObject::tr("learned")) {
        status = Learned;
    }
    else {
        status = New;
    }
}

QString WordStatus::getStatus() const
{
    QString ret = QObject::tr("new");

    switch(status) {
    case New:
        ret = QObject::tr("new");
        break;
    case Middle:
        ret = QObject::tr("middle");
        break;
    case Learned:
        ret = QObject::tr("learned");
        break;
    }

    return ret;
}

int WordStatus::getStatusInt() const
{
    return status;
}

const WordStatus& WordStatus::operator=(const WordStatus &other)
{
    status = other.status;
    return *this;
}

bool WordStatus::operator==(const WordStatus &other) const
{
    return status == other.status;
}

bool WordStatus::operator!=(const WordStatus &other) const
{
    return status != other.status;
}

bool WordStatus::operator>(const WordStatus &other) const
{
    return status > other.status;
}

bool WordStatus::operator<(const WordStatus &other) const
{
    return status < other.status;
}

bool WordStatus::operator>=(const WordStatus &other) const
{
    return status >= other.status;
}

bool WordStatus::operator<=(const WordStatus &other) const
{
    return status <= other.status;
}

WordStatus::WordStatus(const QString s)
    : status(New)
{
    setStatus(s);
}

WordStatus::WordStatus(int s)
    : status(New)
{
    setStatus(s);
}

WordStatus::~WordStatus()
{

}