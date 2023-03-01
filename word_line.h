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

#include <QString>
#include <QObject>
#include <QDate>


class WordLine {
public:
    QString getOriginal() const;
    QString getTranslation() const;
    QString getStatus() const;
    QString getDate(const QString &format = QString()) const;
    QString getDateFormat() const;

    void setOriginal(const QString &pOriginal);
    void setTranslation(const QString &pTranslation);
    void setStatus(const QString &pStatus);
    void setDate(const QString &pDate,
                 const QString &format = QString());
    void setCurrentDate();
    void setDateFormat(const QString &format = QString());

    bool isEmpty() const;

    const WordLine& operator=(const WordLine &other);
    bool operator==(const WordLine &other) const;
    bool operator>(const WordLine &other) const;
    bool operator<(const WordLine &other) const;
    bool operator>=(const WordLine &other) const;
    bool operator<=(const WordLine &other) const;

    WordLine();
    WordLine(const QString &pOriginal,
             const QString &pTranslation = QString(),
             const QString &pStatus = QString(QObject::tr("new")),
             const QString &pDate = QString(),
             const QString &format = QString()
            );
    ~WordLine();
private:
    static QString DefaultDateFormat;

    QString original;
    QString translation;
    QString status;
    QDate date;
    QString dateFormat;
};

inline QDataStream &operator<<(QDataStream &stream, const WordLine &word)
{
    return stream << word.getOriginal() << word.getTranslation()
                  << word.getStatus() << word.getDate();
}

inline QDataStream &operator>>(QDataStream &stream, WordLine &word)
{
    QString original, translation, status, date;
    stream >> original >> translation >> status >> date;
    word = WordLine(original, translation, status, date);

    return stream;
}

#endif
