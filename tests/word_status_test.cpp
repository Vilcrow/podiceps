/**************************************************************************/
/*  word_status_test.cpp                                                  */
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

#include "word_status_test.h"

void WordStatusTest::setAndGetStatus()
{
    WordStatus status;

    // The default value.
    QCOMPARE(status.getStatus(), "new");
    QCOMPARE(status.getStatusInt(), WordStatus::New);

    status.setStatus("middle");
    QCOMPARE(status.getStatus(), "middle");
    QCOMPARE(status.getStatusInt(), WordStatus::Middle);

    status.setStatus("invalid argument");
    QCOMPARE(status.getStatus(), "new");
    QCOMPARE(status.getStatusInt(), WordStatus::New);

    status.setStatus(2);
    QCOMPARE(status.getStatus(), "learned");
    QCOMPARE(status.getStatusInt(), WordStatus::Learned);

    // Invalid value.
    status.setStatus(342);
    QCOMPARE(status.getStatus(), "new");
    QCOMPARE(status.getStatusInt(), WordStatus::New);
}

void WordStatusTest::operators()
{
    WordStatus first;
    WordStatus second;

    QCOMPARE(first.getStatus(), second.getStatus());
    QVERIFY(first == second);

    second.setStatus(WordStatus::Middle);
    QCOMPARE(first == second, false);
    QCOMPARE(first != second, true);
    QCOMPARE(first > second, false);
    QCOMPARE(first >= second, false);
    QCOMPARE(first < second, true);
    QCOMPARE(first <= second, true);

    first = second;
    QVERIFY(first == second);
    QCOMPARE(first == second, true);
    QCOMPARE(first != second, false);
    QCOMPARE(first > second, false);
    QCOMPARE(first >= second, true);
    QCOMPARE(first < second, false);
    QCOMPARE(first <= second, true);
    QCOMPARE(first.getStatus(), second.getStatus());
}
