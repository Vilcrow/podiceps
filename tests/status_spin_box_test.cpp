/**************************************************************************/
/*  status_spin_box_test.cpp                                              */
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

#include "status_spin_box_test.h"
#include "word_status.h"

void StatusSpinBoxTest::inputEvents()
{
    StatusSpinBox spinBox;

    // The default value.
    QCOMPARE(spinBox.getValue(), "new");

    QTest::keyPress(&spinBox, Qt::Key_Up);
    QCOMPARE(spinBox.getValue(), "middle");

    QTest::keyPress(&spinBox, Qt::Key_Up);
    QCOMPARE(spinBox.getValue(), "learned");

    // The maximum value was reached.
    QTest::keyPress(&spinBox, Qt::Key_Up);
    QCOMPARE(spinBox.getValue(), "learned");

    QTest::keyPress(&spinBox, Qt::Key_Down);
    QCOMPARE(spinBox.getValue(), "middle");

    QTest::keyPress(&spinBox, Qt::Key_Down);
    QCOMPARE(spinBox.getValue(), "new");

    // The minimum value was reached.
    QTest::keyPress(&spinBox, Qt::Key_Down);
    QCOMPARE(spinBox.getValue(), "new");
}

void StatusSpinBoxTest::setValue()
{
    StatusSpinBox spinBox;

    // The default value.
    QCOMPARE(spinBox.value(), WordStatus::New);
    QCOMPARE(spinBox.getValue(), "new");

    spinBox.setValue("learned");
    QCOMPARE(spinBox.value(), WordStatus::Learned);

    spinBox.setValue("invalid value");
    QCOMPARE(spinBox.value(), WordStatus::New);
}

void StatusSpinBoxTest::getValue()
{
    StatusSpinBox spinBox;

    // The default value.
    QCOMPARE(spinBox.value(), WordStatus::New);
    QCOMPARE(spinBox.getValue(), "new");

    spinBox.setValue("learned");
    QCOMPARE(spinBox.getValue(), "learned");

    spinBox.setValue("invalid value");
    QCOMPARE(spinBox.getValue(), "new");
}
