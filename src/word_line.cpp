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
    return status.getStatus();
}

QString WordLine::getDate(const QString &format) const
{
    QString ret = QString();

    if(date.isValid()) {
        if(!format.isEmpty()) {
            ret = date.toString(format);
        }
        else {
            ret = date.toString(dateFormat);
        }
    }

    return ret;
}

QString WordLine::getDateFormat() const
{
    return dateFormat;
}

QString WordLine::getComment() const
{
    return comment;
}

QString WordLine::getTranscription() const
{
    return transcription;
}

void WordLine::setOriginal(const QString &pOriginal)
{
    original = pOriginal;
    truncate();
}

void WordLine::setTranslation(const QString &pTranslation)
{
    translation = pTranslation;
    truncate();
}

void WordLine::setStatus(const QString &pStatus)
{
    status = WordStatus(pStatus);
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
    truncate();
}

void WordLine::setComment(const QString &pComment)
{
    comment = pComment;
    truncate();
}

void WordLine::setTranscription(const QString &pTranscription)
{
    transcription = pTranscription;
    truncate();
}

void WordLine::clear()
{
    original = QString();
    translation = QString();
    status = WordStatus(WordStatus::New);
    date = QDate();
    dateFormat = DefaultDateFormat;
    comment = QString();
    transcription = QString();
}

bool WordLine::isEmpty() const
{
    return original.isEmpty() &&
           transcription.isEmpty() &&
           translation.isEmpty() &&
           status.getStatusInt() == WordStatus::New &&
           date.isNull() &&
           comment.isEmpty();
}

bool WordLine::isDateValid() const
{
    return date.isValid();
}

void WordLine::setDomElement(QDomElement &element) const
{
    element.setAttribute("original", original);
    element.setAttribute("translation", translation);
    element.setAttribute("status", status.getStatus());
    element.setAttribute("date", getDate());
    element.setAttribute("dateFormat", dateFormat);
    element.setAttribute("comment", comment);
    element.setAttribute("transcription", transcription);
}

const WordLine& WordLine::operator=(const WordLine &other)
{
    original = other.original;
    translation = other.translation;
    status = other.status;
    date = other.date;
    dateFormat = other.dateFormat;
    comment = other.comment;
    transcription = other.transcription;
    return *this;
}

// The operator is designed to prevent duplication of words in the table.
bool WordLine::operator==(const WordLine &other) const
{
    return original == other.original;
}

bool WordLine::operator!=(const WordLine &other) const
{
    return original != other.original ||
           transcription != other.transcription ||
           translation != other.translation ||
           status != other.status ||
           date != other.date ||
           comment != other.comment;
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

void WordLine::truncate()
{
    original.truncate(MaxOriginalLength);
    translation.truncate(MaxTranslationLength);
    dateFormat.truncate(MaxDateLength);
    comment.truncate(MaxCommentLength);
    transcription.truncate(MaxOriginalLength);
}

WordLine::WordLine(const QString &pOriginal, const QString &pTranslation,
                   const QString &pStatus, const QString &pDate,
                   const QString &format)
    : original(pOriginal), transcription(QString()), translation(pTranslation),
      status(WordStatus(pStatus))
{
    dateFormat = format.isEmpty() ? DefaultDateFormat : format;
    date = QDate::fromString(pDate, dateFormat);
    comment = QString();
    truncate();
}

WordLine::WordLine(const QDomElement &element)
{
    original = element.attribute("original", "");
    translation = element.attribute("translation", "");
    status = WordStatus(element.attribute("status", ""));
    dateFormat = element.attribute("dateFormat", DefaultDateFormat);
    QString dateString = element.attribute("date", "");
    date = QDate::fromString(dateString, dateFormat);
    comment = element.attribute("comment", "");
    transcription = element.attribute("transcription", "");
    truncate();
}

WordLine::~WordLine()
{

}
