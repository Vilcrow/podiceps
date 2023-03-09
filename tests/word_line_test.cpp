/**************************************************************************/
/*  word_line_test.cpp                                                    */
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
#include "CppUTest/TestHarness.h"

TEST_GROUP(EmptyWordLineGroup)
{
    WordLine *word = nullptr;

    void setup()
    {
        // CppUTest detects memory leaks for the QDate object.
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        word = new WordLine();
    }

    void teardown()
    {
        delete word;
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};

TEST(EmptyWordLineGroup, Constructor)
{
    CHECK(word->isEmpty());
}

TEST(EmptyWordLineGroup, GetMethods)
{
    CHECK(word->getOriginal() == "");
    CHECK(word->getTranslation() == "");
    CHECK(word->getStatus() == "");
    CHECK(word->getDate() == "");
    CHECK(word->getDate("dd-MM-yyyy") == "");
    // Default format.
    CHECK(word->getDateFormat() == "yyyy-MM-dd");
    CHECK(word->getComment() == "");
}

TEST(EmptyWordLineGroup, SetMethods)
{
    word->setOriginal("world");
    CHECK(word->getOriginal() == "world");

    word->setTranslation("мир");
    CHECK(word->getTranslation() == "мир");

    word->setStatus("remembered");
    CHECK(word->getStatus() == "remembered");

    word->setDate("12-10-2012", "dd-MM-yyyy");
    CHECK(word->getDate() == "12-10-2012");
    CHECK(word->getDateFormat() == "dd-MM-yyyy");

    word->setDateFormat("MM.dd.yyyy");
    CHECK(word->getDate() == "10.12.2012");
    CHECK(word->getDate("yyyy") == "2012");

    word->setComment("Hello, world!");
    CHECK(word->getComment() == "Hello, world!");
}

TEST(EmptyWordLineGroup, MaxLength)
{
    word->setOriginal(QString(2 * WordLine::MaxOriginalLength, 'o'));
    CHECK_EQUAL(word->getOriginal().length(), WordLine::MaxOriginalLength);

    word->setTranslation(QString(2 * WordLine::MaxTranslationLength, 't'));
    CHECK_EQUAL(word->getTranslation().length(), WordLine::MaxTranslationLength);

    word->setStatus(QString(2 * WordLine::MaxStatusLength, 's'));
    CHECK_EQUAL(word->getStatus().length(), WordLine::MaxStatusLength);

    word->setDateFormat(QString(2 * WordLine::MaxDateLength, 'f'));
    CHECK_EQUAL(word->getDateFormat().length(), WordLine::MaxDateLength);

    word->setComment(QString(2 * WordLine::MaxCommentLength, 'c'));
    CHECK_EQUAL(word->getComment().length(), WordLine::MaxCommentLength);
}

TEST(EmptyWordLineGroup, InvalidDate)
{
    CHECK_FALSE(word->isDateValid());

    word->setDate("10-10-2010", "dd-MM-yyyy");
    CHECK(word->isDateValid());

    word->setDate("-10-2010", "dd-MM-yyyy");
    CHECK(word->isDateValid());
    CHECK(word->getDate() == "10-10-2010");

    word->clear();
    CHECK_FALSE(word->isDateValid());
}

TEST_GROUP(FullWordLineGroup)
{
    WordLine *word = nullptr;

    void setup()
    {
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        word = new WordLine("word", "слово", "new",
                            "01-01-2023", "dd-MM-yyyy");
        word->setComment("Comment.");
    }

    void teardown()
    {
        delete word;
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};

TEST(FullWordLineGroup, Constructor)
{
    CHECK_FALSE(word->isEmpty());
    CHECK(word->isDateValid());
}

TEST(FullWordLineGroup, GetMethods)
{
    CHECK(word->getOriginal() == "word");
    CHECK(word->getTranslation() == "слово");
    CHECK(word->getStatus() == "new");
    CHECK(word->getDate() == "01-01-2023");
    CHECK(word->getDateFormat() == "dd-MM-yyyy");
    CHECK(word->getComment() == "Comment.");
}

TEST(FullWordLineGroup, ClearMethod)
{
    word->clear();
    CHECK(word->isEmpty());
}

TEST(FullWordLineGroup, Operators)
{
    WordLine other("word");
    CHECK(*word == other);
    CHECK(*word != other);
    other.setOriginal("world");
    CHECK_FALSE(*word == other);
    CHECK(*word != other);
    CHECK(*word < other);
    CHECK_FALSE(*word > other);
    CHECK(*word <= other);
    CHECK_FALSE(*word >= other);
}

TEST_GROUP(WordLineFromQDomElementGroup)
{
    WordLine *word = nullptr;
    QDomElement *element = nullptr;

    void setup()
    {
        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
        QDomDocument doc;
        element = new QDomElement();
        *element = doc.createElement("word");
        doc.appendChild(*element);
        element->setAttribute("original", "world");
        element->setAttribute("translation", "мир");
        element->setAttribute("status", "new");
        element->setAttribute("date", "03-11-2023");
        element->setAttribute("dateFormat", "MM-dd-yyyy");
        element->setAttribute("comment", "Any comment.");

        word = new WordLine(*element);
    }

    void teardown()
    {
        delete word;
        delete element;
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};

TEST(WordLineFromQDomElementGroup, Constructor)
{
    CHECK_FALSE(word->isEmpty());
}

TEST(WordLineFromQDomElementGroup, GetMethods)
{
    CHECK(word->getOriginal() == "world");
    CHECK(word->getTranslation() == "мир");
    CHECK(word->getStatus() == "new");
    CHECK(word->getDate() == "03-11-2023");
    CHECK(word->getDateFormat() == "MM-dd-yyyy");
    CHECK(word->getComment() == "Any comment.");
}

TEST(WordLineFromQDomElementGroup, SetDomElement)
{
    QDomDocument doc;
    QDomElement e = doc.createElement("word");
    doc.appendChild(e);
    word->setDomElement(e);

    CHECK(e.attribute("original") =="world");
    CHECK(e.attribute("translation") == "мир");
    CHECK(e.attribute("status") == "new");
    CHECK(e.attribute("date") == "03-11-2023");
    CHECK(e.attribute("dateFormat") == "MM-dd-yyyy");
    CHECK(e.attribute("comment") == "Any comment.");
    CHECK(e.attribute("trash") == "");
}
