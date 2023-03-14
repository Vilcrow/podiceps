/**************************************************************************/
/*  word_status.h                                                         */
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

#ifndef WORD_STATUS_VIL_H
#define WORD_STATUS_VIL_H

#include <QColor>
#include <QMap>
#include <QString>

class WordStatus {
public:
    enum Status { New, Middle, Learned };

    static QColor getColor(int i);

    void setStatus(int i);
    void setStatus(const QString s);

    QString getStatus() const;
    int getStatusInt() const;

    const WordStatus& operator=(const WordStatus &other);
    bool operator==(const WordStatus &other) const;
    bool operator!=(const WordStatus &other) const;
    bool operator>(const WordStatus &other) const;
    bool operator<(const WordStatus &other) const;
    bool operator>=(const WordStatus &other) const;
    bool operator<=(const WordStatus &other) const;

    WordStatus(const QString s = QString());
    WordStatus(int s);
    virtual ~WordStatus();
private:
    Status status;

    static QMap<int, QColor> map;
};

#endif
