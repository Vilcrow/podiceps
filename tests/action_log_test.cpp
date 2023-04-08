/**************************************************************************/
/*  action_log_test.cpp                                                   */
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

#include "action_log_test.h"
#include "action_add.h"

void ActionLogTest::addAction()
{
    ActionLog log;
    QVERIFY(log.isEmpty());

    ActionAdd *addAct = new ActionAdd(WordLine());
    QVERIFY(log.addAction(addAct));
    QCOMPARE(log.isEmpty(), false);
    // Try to add again.
    QCOMPARE(log.addAction(addAct), false);
}

void ActionLogTest::erase()
{
    ActionLog log;
    QVERIFY(log.isEmpty());

    ActionAdd *addAct = new ActionAdd(WordLine());
    QVERIFY(log.addAction(addAct));

    // Invalid arguments.
    log.erase(0, 1);
    QCOMPARE(log.isEmpty(), false);
    log.erase(-1, 0);
    QCOMPARE(log.isEmpty(), false);
    log.erase(-10, 50);
    QCOMPARE(log.isEmpty(), false);
    // Valid arguments.
    log.erase(0, 0);
    QVERIFY(log.isEmpty());
}
