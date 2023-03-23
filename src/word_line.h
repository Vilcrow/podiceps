/**************************************************************************/
/*  word_line.h                                                           */
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

#ifndef WORD_LINE_VIL_H
#define WORD_LINE_VIL_H

#include "word_status.h"
#include <QDate>
#include <QDomElement>
#include <QObject>
#include <QString>

class WordLine {
public:
    enum { MaxOriginalLength = 50, MaxTranslationLength = 100,
           MaxDateLength = 20, MaxCommentLength = 500 };

    QString getOriginal() const;
    QString getTranslation() const;
    QString getStatus() const;
    QString getDate(const QString &format = QString()) const;
    QString getDateFormat() const;
    QString getComment() const;
    QString getTranscription() const;

    void setOriginal(const QString &pOriginal);
    void setTranslation(const QString &pTranslation);
    void setStatus(const QString &pStatus);
    void setDate(const QString &pDate,
                 const QString &format = QString());
    void setCurrentDate();
    void setDateFormat(const QString &format);
    void setComment(const QString &pComment);
    void setTranscription(const QString &pTranscription);
    void clear();

    bool isEmpty() const;
    bool isDateValid() const;
    bool isSame(const WordLine &other) const;

    void setDomElement(QDomElement &element) const;

    const WordLine& operator=(const WordLine &other);
    bool operator==(const WordLine &other) const;
    bool operator!=(const WordLine &other) const;
    bool operator>(const WordLine &other) const;
    bool operator<(const WordLine &other) const;
    bool operator>=(const WordLine &other) const;
    bool operator<=(const WordLine &other) const;

    WordLine(const QString &pOriginal = QString(),
             const QString &pTranslation = QString(),
             const QString &pStatus = QString(),
             const QString &pDate = QString(),
             const QString &format = DefaultDateFormat
            );
    WordLine(const QDomElement &element);
    ~WordLine();
private:
    QString original;
    QString transcription;
    QString translation;
    WordStatus status;
    QDate date;
    QString dateFormat;
    QString comment;

    static QString DefaultDateFormat;

    void truncate();
};

#endif
