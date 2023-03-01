/**************************************************************************/
/*  word_line.cpp                                                         */
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

#include "word_line.h"
#include <QDate>

QString WordLine::DefaultDateFormat = "yyyy-MM-dd";

QString WordLine::getOriginal() const
{
    return original;
}

QString WordLine::getTranslation() const
{
    return translation;
}

QString WordLine::getStatus() const
{
    return status;
}

QString WordLine::getDate(const QString &format) const
{
    if(!format.isEmpty()) {
        return date.toString(format);
    }
    else {
        return date.toString(dateFormat);
    }
}

QString WordLine::getDateFormat() const
{
    return dateFormat;
}

void WordLine::setOriginal(const QString &pOriginal)
{
    original = pOriginal;
}

void WordLine::setTranslation(const QString &pTranslation)
{
    translation = pTranslation;
}

void WordLine::setStatus(const QString &pStatus)
{
    status = pStatus;
}

void WordLine::setDate(const QString &pDate, const QString &format)
{
    QDate d;

    if(format.isEmpty()) {
        d = QDate::fromString(pDate, dateFormat);
    }
    else {
        d = QDate::fromString(pDate, format);
    }

    if(d.isValid()) {
        date = d;
    }
    else {
        // Set the warning message.
    }
}

void WordLine::setCurrentDate()
{
    date = QDate::currentDate();
}

void WordLine::setDateFormat(const QString &format)
{
    dateFormat = format;
}

bool WordLine::isEmpty() const
{
    return original.isEmpty() && translation.isEmpty() &&
           status.isEmpty() && date.isNull();
}

const WordLine& WordLine::operator=(const WordLine &other)
{
    original = other.original;
    translation = other.translation;
    status = other.status;
    date = other.date;
    dateFormat = other.dateFormat;
    return *this;
}

bool WordLine::operator==(const WordLine &other) const
{
    return original == other.original;
}

bool WordLine::operator>(const WordLine &other) const
{
    return original > other.original;
}

bool WordLine::operator<(const WordLine &other) const
{
    return original < other.original;
}

bool WordLine::operator>=(const WordLine &other) const
{
    return original >= other.original;
}

bool WordLine::operator<=(const WordLine &other) const
{
    return original <= other.original;
}

WordLine::WordLine()
{
    dateFormat = DefaultDateFormat;
}

WordLine::WordLine(const QString &pOriginal, const QString &pTranslation,
                   const QString &pStatus, const QString &pDate,
                   const QString &format)
{
    original = pOriginal;
    translation = pTranslation;
    status = pStatus;
    dateFormat = format.isEmpty() ? DefaultDateFormat : format;
    date = QDate::fromString(pDate, dateFormat);
}

WordLine::~WordLine()
{

}
