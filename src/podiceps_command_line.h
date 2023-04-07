/**************************************************************************/
/*  podiceps_command_line.h                                               */
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

#ifndef PODICEPS_COMMAND_LINE_VIL_H
#define PODICEPS_COMMAND_LINE_VIL_H

#include "word_line.h"
#include <QCoreApplication>
#include <QCommandLineOption>
#include <QFileInfo>
#include <QList>

struct Arguments {
    QString oldOriginal;
    QString original;
    QString transcription;
    QString translation;
    QString status;
    QString date;
    QString dateFormat;
    QString comment;

    int lineNumber;
    QString printFormat;

    QString fileName;

    bool reverse;
};

struct MaxLength {
    int original;
    int transcription;
    int translation;
    int status;
    int date;
    int comment;
};

class QCommandLineParser;

class PodicepsCommandLine : public QCoreApplication {
    Q_OBJECT
public:
    int run();

    PodicepsCommandLine(int &argc, char **argv);
    virtual ~PodicepsCommandLine();
private:
    enum ParseResult { Error, VersionRequested, HelpRequested, Ok };

    QCommandLineParser *parser;
    QString errorMessage;
    Arguments args;

    QList<WordLine> words;

    void setupParser();
    ParseResult parse();

    void setLanguage();
    bool setLanguage(const QString &lang);

    void printAbout() const;
    void printUsage() const;
    void printInfo(const QFileInfo &fileInfo) const;
    void printStatistics() const;

    bool setWords();
    void setArguments();
    void setArgumentsDefault();

    bool writeToFile();

    void addWord(bool *ok);
    void deleteWord(bool *ok);
    void editWord(bool *ok);
    void printWords(bool *ok);

    void setPrintableWords(QList<WordLine> &rWords) const;
    void setMaxLength(const QList<WordLine> &pWords, MaxLength &len) const;
    void setPrintStrings(const QList<WordLine> &pWords,
                         QList<QString> &rStrings,
                         const MaxLength &len) const;
    bool isValidFormat(const QString &format) const;

    static QString defaultOutputFormat;
};

#endif
