/**************************************************************************/
/*  main_test.cpp                                                         */
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
#include "word_edit_test.h"
#include "word_line_test.h"
#include "word_status_test.h"
#include <QTest>
#include <QApplication>

int main(int argc, char **argv)
{
    // For the GUI tests.
    QApplication app(argc, argv);

    int status = 0;

    // StatusSpinBox.
    {
        StatusSpinBoxTest test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // WordEdit.
    {
        WordEditTest test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // WordLine.
    {
        WordLineTest test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // WordStatus.
    {
        WordStatusTest test;
        status |= QTest::qExec(&test, argc, argv);
    }

    return status;
}
