/**************************************************************************/
/*  word_line_test.h                                                      */
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

#ifndef WORD_LINE_TEST_VIL_H
#define WORD_LINE_TEST_VIL_H

#include "word_line.h"
#include <QTest>

Q_DECLARE_METATYPE(QDomElement);
Q_DECLARE_METATYPE(WordLine);

class WordLineTest : public QObject {
    Q_OBJECT
private slots:
    void getMethods();
    void getMethods_data();
    void setMethods();
    void setMethods_data();
    void isEmptyMethod();
    void isEmptyMethod_data();
    void clearMethod();
    void clearMethod_data();
    void maxLength();
    void maxLength_data();
    void operators();
    void operators_data();
    void setDomElement();
    void setDomElement_data();
};

#endif
