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

#include "word_line_test.h"

void WordLineTest::getMethods()
{
    QFETCH(WordLine, word);
    QFETCH(QString, original);
    QFETCH(QString, transcription);
    QFETCH(QString, translation);
    QFETCH(QString, status);
    QFETCH(QString, date);
    QFETCH(QString, dateFormat);
    QFETCH(QString, comment);

    QCOMPARE(word.getOriginal(), original);
    QCOMPARE(word.getTranscription(), transcription);
    QCOMPARE(word.getTranslation(), translation);
    QCOMPARE(word.getStatus(), status);
    QCOMPARE(word.getDate(), date);
    QCOMPARE(word.getDateFormat(), dateFormat);
    QCOMPARE(word.getComment(), comment);
}

void WordLineTest::getMethods_data()
{
    QTest::addColumn<WordLine>("word");
    QTest::addColumn<QString>("original");
    QTest::addColumn<QString>("transcription");
    QTest::addColumn<QString>("translation");
    QTest::addColumn<QString>("status");
    QTest::addColumn<QString>("date");
    QTest::addColumn<QString>("dateFormat");
    QTest::addColumn<QString>("comment");

    QString defaultDateFormat = "yyyy-MM-dd";

    WordLine emptyWord;
    QTest::newRow("Empty") << emptyWord
                           << ""
                           << ""
                           << ""
                           << "new"
                           << ""
                           << defaultDateFormat
                           << "";

    WordLine fullWord("power", "сила", "learned", "01-03-2022", "dd-MM-yyyy");
    fullWord.setTranscription("ˈpaʊə");
    fullWord.setComment("Comment.");
    QTest::newRow("Full") << fullWord
                          << "power"
                          << "ˈpaʊə"
                          << "сила"
                          << "learned"
                          << "01-03-2022"
                          << "dd-MM-yyyy"
                          << "Comment.";

    QDomDocument doc;
    QDomElement element = doc.createElement("word");
    doc.appendChild(element);
    element.setAttribute("original", "world");
    element.setAttribute("translation", "мир");
    element.setAttribute("status", "learned");
    element.setAttribute("date", "03-11-2023");
    element.setAttribute("dateFormat", "MM-dd-yyyy");
    element.setAttribute("comment", "Any comment.");
    element.setAttribute("transcription", "wɜːld");

    WordLine xmlWord(element);
    QTest::newRow("Xml") << xmlWord
                         << "world"
                         << "wɜːld"
                         << "мир"
                         << "learned"
                         << "03-11-2023"
                         << "MM-dd-yyyy"
                         << "Any comment.";
}

void WordLineTest::setMethods()
{
    QFETCH(WordLine, word);
    QFETCH(QString, original);
    QFETCH(QString, transcription);
    QFETCH(QString, translation);
    QFETCH(QString, status);
    QFETCH(QString, date);
    QFETCH(QString, dateFormat);
    QFETCH(QString, comment);

    QCOMPARE(word.getOriginal(), original);
    QCOMPARE(word.getTranscription(), transcription);
    QCOMPARE(word.getTranslation(), translation);
    QCOMPARE(word.getStatus(), status);
    QCOMPARE(word.getDate(), date);
    QCOMPARE(word.getDateFormat(), dateFormat);
    QCOMPARE(word.getComment(), comment);
}

void WordLineTest::setMethods_data()
{
    QTest::addColumn<WordLine>("word");
    QTest::addColumn<QString>("original");
    QTest::addColumn<QString>("transcription");
    QTest::addColumn<QString>("translation");
    QTest::addColumn<QString>("status");
    QTest::addColumn<QString>("date");
    QTest::addColumn<QString>("dateFormat");
    QTest::addColumn<QString>("comment");

    QString defaultDateFormat = "yyyy-MM-dd";

    WordLine word;
    word.setOriginal("orange");
    word.setTranscription("ˈɒrɪnʤ");
    word.setTranslation("оранжевый, апельсин");
    word.setStatus("learned");
    word.setDate("20-02-2020", "dd-MM-yyyy");
    word.setDateFormat("yyyy");
    word.setComment("Comment.");
    QTest::newRow("Empty") << word
                           << "orange"
                           << "ˈɒrɪnʤ"
                           << "оранжевый, апельсин"
                           << "learned"
                           << "2020"
                           << "yyyy"
                           << "Comment.";
}

void WordLineTest::isEmptyMethod()
{
    QFETCH(bool, result);
    QFETCH(bool, expected);

    QCOMPARE(result, expected);
}

void WordLineTest::isEmptyMethod_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("expected");

    WordLine word;
    QTest::newRow("empty") << word.isEmpty() << true;

    word.setOriginal("hello");
    QTest::newRow("with-original") << word.isEmpty() << false;
}

void WordLineTest::clearMethod()
{
    QFETCH(bool, result);
    QFETCH(bool, expected);

    QCOMPARE(result, expected);
}

void WordLineTest::clearMethod_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("expected");

    WordLine word("hello");
    QTest::newRow("with-original-not-empty") << word.isEmpty() << false;
    word.clear();
    QTest::newRow("with-original-cleared") << word.isEmpty() << true;

    word.setTranscription("t");
    QTest::newRow("with-transcription-not-empty") << word.isEmpty() << false;
    word.clear();
    QTest::newRow("with-transcription-cleared") << word.isEmpty() << true;

    word.setTranslation("t");
    QTest::newRow("with-translation-not-empty") << word.isEmpty() << false;
    word.clear();
    QTest::newRow("with-translation-cleared") << word.isEmpty() << true;

    WordLine fullWord("apple", "яблоко", "middle", "2020-10-01", "yyyy-dd-MM");
    fullWord.setTranscription("æpl");
    fullWord.setComment("Any comment.");
    QTest::newRow("full-not-empty") << fullWord.isEmpty() << false;
    fullWord.clear();
    QTest::newRow("full-cleared") << fullWord.isEmpty() << true;
}

void WordLineTest::maxLength()
{
    QFETCH(WordLine, word);
    QFETCH(int, original);
    QFETCH(int, transcription);
    QFETCH(int, translation);
    QFETCH(int, status);
    QFETCH(int, date);
    QFETCH(int, dateFormat);
    QFETCH(int, comment);

    QCOMPARE(word.getOriginal().length(), original);
    QCOMPARE(word.getTranscription().length(), transcription);
    QCOMPARE(word.getTranslation().length(), translation);
    QCOMPARE(word.getStatus().length(), status);
    QCOMPARE(word.getDate().length(), date);
    QCOMPARE(word.getDateFormat().length(), dateFormat);
    QCOMPARE(word.getComment().length(), comment);
}

void WordLineTest::maxLength_data()
{
    QTest::addColumn<WordLine>("word");
    QTest::addColumn<int>("original");
    QTest::addColumn<int>("transcription");
    QTest::addColumn<int>("translation");
    QTest::addColumn<int>("status");
    QTest::addColumn<int>("date");
    QTest::addColumn<int>("dateFormat");
    QTest::addColumn<int>("comment");

    int maxOriginal = WordLine::MaxOriginalLength;
    int maxTranscription = WordLine::MaxOriginalLength;
    int maxTranslation = WordLine::MaxTranslationLength;
    int maxDate = WordLine::MaxDateLength;
    int maxComment = WordLine::MaxCommentLength;

    WordLine word;
    int defaultDate = word.getDateFormat().length();
    QTest::newRow("zero") << word
                          << 0
                          << 0
                          << 0
                          << 3
                          << 0
                          << defaultDate
                          << 0;

    word.setOriginal(QString(2 * WordLine::MaxOriginalLength, 'o'));
    word.setTranscription(QString(2 * WordLine::MaxOriginalLength, 't'));
    word.setTranslation(QString(2 * WordLine::MaxTranslationLength, 't'));
    word.setStatus("invalid argument");
    word.setDateFormat(QString(2 * WordLine::MaxDateLength, 'f'));
    word.setComment(QString(2 * WordLine::MaxCommentLength, 'c'));

    QTest::newRow("full") << word
                          << maxOriginal
                          << maxTranscription
                          << maxTranslation
                          << 3
                          << 0
                          << maxDate
                          << maxComment;
}

void WordLineTest::operators()
{
    QFETCH(bool, statement);
    QFETCH(bool, result);

    QCOMPARE(statement, result);
}

void WordLineTest::operators_data()
{
    QTest::addColumn<bool>("statement");
    QTest::addColumn<bool>("result");

    WordLine word;
    WordLine otherWord("other");
    QTest::newRow("different ==") << (word == otherWord) << false;
    QTest::newRow("different !=") << (word != otherWord) << true;
    QTest::newRow("different >") << (word > otherWord) << false;
    QTest::newRow("different >=") << (word >= otherWord) << false;
    QTest::newRow("different <") << (word < otherWord) << true;
    QTest::newRow("different <=") << (word <= otherWord) << true;

    word.setOriginal("other");
    QTest::newRow("same ==") << (word == otherWord) << true;
    QTest::newRow("same !=") << (word != otherWord) << false;
    QTest::newRow("same >") << (word > otherWord) << false;
    QTest::newRow("same >=") << (word >= otherWord) << true;
    QTest::newRow("same <") << (word < otherWord) << false;
    QTest::newRow("same <=") << (word <= otherWord) << true;
}

void WordLineTest::setDomElement()
{
    QFETCH(QDomElement, element);
    QFETCH(WordLine, word);

    QCOMPARE(element.attribute("original"), word.getOriginal());
    QCOMPARE(element.attribute("transcription"), word.getTranscription());
    QCOMPARE(element.attribute("translation"), word.getTranslation());
    QCOMPARE(element.attribute("status"), word.getStatus());
    QCOMPARE(element.attribute("date"), word.getDate());
    QCOMPARE(element.attribute("dateFormat"), word.getDateFormat());
    QCOMPARE(element.attribute("comment"), word.getComment());
}

void WordLineTest::setDomElement_data()
{
    QTest::addColumn<QDomElement>("element");
    QTest::addColumn<WordLine>("word");

    QDomDocument doc;
    QDomElement element = doc.createElement("word");
    doc.appendChild(element);

    WordLine fullWord("word", "слово", "middle", "2023-03-01", "yyyy-MM-dd");
    fullWord.setTranscription("wɜːd");
    fullWord.setComment("Hello.");

    fullWord.setDomElement(element);
    QTest::newRow("Full") << element << fullWord;
}
