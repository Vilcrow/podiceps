/**************************************************************************/
/*  word_edit_test.cpp                                                    */
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

#include "word_edit_test.h"
#include "word_line.h"

void WordEditTest::setAndGetWord()
{
    WordEdit wordEdit;
    QVERIFY(wordEdit.getWord().isEmpty());

    WordLine word("dream", "мечта", "learned", "04-11-2022", "dd-MM-yyyy");
    word.setTranscription("driːm");
    word.setComment("Comment.");
    wordEdit.setWord(word);
    QCOMPARE(wordEdit.getWord().getOriginal(), "dream");
    QCOMPARE(wordEdit.getWord().getTranscription(), "driːm");
    QCOMPARE(wordEdit.getWord().getTranslation(), "мечта");
    QCOMPARE(wordEdit.getWord().getStatus(), "learned");
    QCOMPARE(wordEdit.getWord().getDate("dd-MM-yyyy"), "04-11-2022");
    QCOMPARE(wordEdit.getWord().getComment(), "Comment.");
}
