/**************************************************************************/
/*  podiceps_command_line.cpp                                             */
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

#include "podiceps_command_line.h"
#include "settings.h"
#include "table_model.h"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>
#include <QDomElement>
#include <QFile>
#include <QMap>
#include <QModelIndex>
#include <QTextStream>
#include <QTranslator>

// Original, Transcription, Translation, Status, Date, Comment.
QString PodicepsCommandLine::defaultOutputFormat = "oTtSDC";

int PodicepsCommandLine::run()
{
    ParseResult result = parse();

    switch(result) {
    case Error:
        printf("%s\n", errorMessage.toStdString().c_str());
        return 1;
    case VersionRequested:
        printf("podiceps version %s, Qt version %s\n",
            applicationVersion().toStdString().c_str(), QT_VERSION_STR);
        break;
    case HelpRequested:
        parser->showHelp();
        break;
    case Ok:
        break;
    }

    return 0;
}

void PodicepsCommandLine::setupParser()
{
    parser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser->setApplicationDescription(tr("Run without options to launch the GUI"));

    parser->addOptions({{"a", tr("Add a new word")},
                        {"d", tr("Delete a word"), tr("original")},
                        {"e", tr("Edit a word"), tr("original")},
                        {"p", tr("Print a words")},
                        {"f", tr("Set the output format"), tr("format")},
                        {"n", tr("Number of output lines"), tr("number")},
                        {"r", tr("Set the reverse order")},
                        {"s", tr("Show the statistics")},
                        {"F", tr("Specify the dictionary file"),
                              tr("path-to-file")},
                        {"i", tr("Info about the dictionary file")},
                        {"o", tr("Original of the word"), tr("original")},
                        {"T", tr("Transcription of the word"),
                              tr("transcription")},
                        {"t", tr("Translation of the word"), tr("translation")},
                        {"S", tr("Status of the word (new, middle, learned)"),
                              tr("status")},
                        {"D", tr("Date of the word")},
                        {"m", tr("Date format"), tr("format")},
                        {"C", tr("Comment for the word"), tr("comment")},
                        {"l", tr("Set a language of the program"),
                              tr("language")},
                        {"I", tr("Info about the program")},
                        {{"h", "help"}, tr("Display help")},
                        {{"u", "help-usage"}, tr("Display usage help")},
                        {{"v", "version"}, tr("Display version information")}});
}

PodicepsCommandLine::ParseResult PodicepsCommandLine::parse()
{
    if(!parser->parse(arguments())) {
        errorMessage = parser->errorText();
        return Error;
    }

    if(parser->isSet("I")) {
        printAbout();
        return Ok;
    }

    if(parser->isSet("h")) {
        return HelpRequested;
    }

    if(parser->isSet("u")) {
        printUsage();
        return Ok;
    }

    if(parser->isSet("v")) {
        return VersionRequested;
    }

    if(parser->isSet("l")) {
        if(setLanguage(parser->value("l"))) {
            return Ok;
        }
        else {
            return Error;
        }
    }

    if(parser->isSet("f")) {
        if(!isValidFormat(parser->value("f"))) {
            errorMessage = tr("Specified invalid output format");
            return Error;
        }
    }

    if(parser->isSet("r")) {
        args.reverse = true;
    }

    if(parser->isSet("F")) {
        args.fileName = parser->value("F");
        Settings::setLastFileName(args.fileName);
        return Ok;
    }

    // Show info about the file.
    if(parser->isSet("i")) {
        if(args.fileName.isEmpty()) {
            errorMessage = tr("The dictionary file isn't specified.");
            return Error;
        }
        QFileInfo fileInfo(args.fileName);
        if(!fileInfo.exists()) {
            errorMessage = tr("The specified file doesn't exist: %1")
                                                  .arg(args.fileName);
            return Error;
        }

        printInfo(fileInfo);
        return Ok;
    }

    // Show the statistics.
    if(parser->isSet("s")) {
        if(args.fileName.isEmpty()) {
            errorMessage = tr("The dictionary file isn't specified.");
            return Error;
        }

        if(!setWords()) {
            return Error;
        }
        else {
            printStatistics();
            return Ok;
        }
    }

    // Add a word
    if(parser->isSet("a")) {
        bool ok;
        addWord(&ok);
        if(!ok) {
            return Error;
        }
        return Ok;
    }

    // Delete a word
    if(parser->isSet("d")) {
        args.original = parser->value("d");
        bool ok;
        deleteWord(&ok);
        if(!ok) {
            return Error;
        }
        return Ok;
    }

    // Edit a word
    if(parser->isSet("e")) {
        args.oldOriginal = parser->value("e");
        bool ok;
        editWord(&ok);
        if(!ok) {
            return Error;
        }
        return Ok;
    }

    // Print words
    if(parser->isSet("p")) {
        bool ok;
        printWords(&ok);
        if(!ok) {
            return Error;
        }
        return Ok;
    }

    return Ok;
}

void PodicepsCommandLine::setLanguage()
{
    Settings::Language appLanguage = Settings::getLanguage();

    QTranslator *translator = new QTranslator;
    QString path;
    switch(appLanguage) {
    case Settings::RussianLang:
        path = ":/podiceps_ru.qm";
        break;
    default:
        path = "";
        break;
    }

    if(!path.isEmpty()) {
        if(translator->load(path)) {
            installTranslator(translator);
        }
        else {
            qDebug() << QString("Unable to open the translation file: %1")
                                                                .arg(path);
        }
    }
}

bool PodicepsCommandLine::setLanguage(const QString &lang)
{
    bool result = false;

    QMap<int, QString> langs = Settings::getAvailableLangs();
    for(int l : langs.keys()) {
        if(langs[l] == lang) {
            Settings::setLanguage(static_cast<Settings::Language>(l));
            result = true;
        }
    }

    if(!result) {
        QString msg = tr("Invalid language. Available:\n");
        for(int l : langs.keys()) {
            msg += langs[l] + "\n";
        }

        errorMessage = msg;
    }

    return result;
}

void PodicepsCommandLine::printAbout() const
{
    QString nameVersion = QString("%1 %2\n").arg(applicationName())
                                            .arg(PODICEPS_VERSION);
    QString copyrightChar = QChar(0x00A9);
    QString copyright = tr("Copyright") + copyrightChar +
                           " 2022-2023 ";
    QString author = "Vilnur Shakirov\n";
    QString email = tr("Email: podiceps-feedback@vilcrow.net\n");
    QString license = tr("License: GPLv3. See: http://www.gnu.org/licenses/\n");
    QString description = tr("A simple program for maintaining "
                             "a dictionary of foreign words.\n");

    QString total = nameVersion + copyright + author + email + license
                                                     + description;
    std::cout << total.toStdString();
}

void PodicepsCommandLine::printUsage() const
{
    QList<QString> text;

    text << tr("Add a new word:\n");
    text << tr("podiceps -a -o <original> -t <translation> ...\n");
    text << tr("(the <original> is required)\n\n");

    text << tr("Delete a word:\n");
    text << tr("podiceps -d <original>\n\n");

    text << tr("Edit a word:\n");
    text << tr("podiceps -e <original> -t <translation> ...\n");
    text << tr("(this is a change of the <translation>)\n\n");

    text << tr("Print a words:\n");
    text << tr("podiceps -p -o <original> ... -n 4 -f otD\n");
    text << tr("(find words matching the filter <original> and "
               "print the original, translation and date for 4 of them)\n\n");

    text << tr("Set columns for output:\n");
    text << tr("podiceps -p ... -f <format>\n");
    text << tr("(available chars: o, T, t, S, D. Example: otD)\n\n");

    for(const QString &str : text) {
        std::cout << str.toStdString();
    }
}

void PodicepsCommandLine::printInfo(const QFileInfo &fileInfo) const
{
    QString name = fileInfo.fileName();
    QString path = fileInfo.filePath();

    QDateTime lastModifiedTime = fileInfo.lastModified();
    QString date = lastModifiedTime.toString("yyyy-MM-dd");
    QString time = lastModifiedTime.toString("HH:mm:ss");

    printf("%-10s%s\n", tr("Name:").toStdString().c_str(),
                               name.toStdString().c_str());
    printf("%-10s%s\n", tr("Path:").toStdString().c_str(),
                               path.toStdString().c_str());
    printf("%-10s%s\n", tr("Date:").toStdString().c_str(),
                               date.toStdString().c_str());
    printf("%-10s%s\n", tr("Time:").toStdString().c_str(),
                               time.toStdString().c_str());
}

void PodicepsCommandLine::printStatistics() const
{
    TableModel model(words);
    int totalCount = model.rowCount(QModelIndex());
    int newCount = model.getWordsCount(WordStatus::New);
    int middleCount = model.getWordsCount(WordStatus::Middle);
    int learnedCount = model.getWordsCount(WordStatus::Learned);

    double newPercent = (double)newCount / (double)totalCount * 100;
    double middlePercent = (double)middleCount / (double)totalCount * 100;
    double learnedPercent = (double)learnedCount / (double)totalCount * 100;

    printf("%-10s%d (%.2f%%)\n", tr("New:").toStdString().c_str(),
                                        newCount, newPercent);
    printf("%-10s%d (%.2f%%)\n", tr("Middle:").toStdString().c_str(),
                                        middleCount, middlePercent);
    printf("%-10s%d (%.2f%%)\n", tr("Learned:").toStdString().c_str(),
                                        learnedCount, learnedPercent);
    printf("%-10s%d\n", tr("Total:").toStdString().c_str(), totalCount);
}

bool PodicepsCommandLine::setWords()
{
    QFile xmlFile(args.fileName);
    if(!xmlFile.open(QIODevice::ReadOnly)) {
        errorMessage = tr("Unable to open file: %1").arg(args.fileName);
        return false;
    }

    QDomDocument document;
    document.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = document.documentElement();
    QDomElement node = root.firstChild().toElement();

    while(!node.isNull()) {
        if(node.tagName() == "word") {
            while(!node.isNull()) {
                WordLine word(node);
                words.append(word);
                node = node.nextSibling().toElement();
            }
        }
        node = node.nextSibling().toElement();
    }

    if(words.isEmpty()) {
        errorMessage = tr("The file contains no words or "
                          "has an invalid format.");
        return false;
    }

    std::sort(words.begin(), words.end());

    return true;
}

void PodicepsCommandLine::setArguments()
{
    args.original = parser->value("o");
    args.transcription = parser->value("T");
    args.translation = parser->value("t");
    args.status = parser->value("S");
    args.date = parser->value("D");
    args.dateFormat = parser->value("m");
    args.comment = parser->value("C");

    if(parser->isSet("F")) {
        args.fileName = parser->value("F");
    }
    else {
        args.fileName = Settings::getLastFileName();
    }

    if(parser->isSet("f")) {
        QString format = parser->value("f");
        if(isValidFormat(format)) {
            args.printFormat = format;
        }
        else {
            args.printFormat = defaultOutputFormat;
        }
    }

    if(parser->isSet("n")) {
        bool ok;
        int n = parser->value("n").toInt(&ok);
        if(ok) {
            args.lineNumber = n;
        }
    }

    if(parser->isSet("r")) {
        args.reverse = true;
    }
}

void PodicepsCommandLine::setArgumentsDefault()
{
    args.oldOriginal = QString();
    args.original = QString();
    args.transcription = QString();
    args.translation = QString();
    args.status = QString();
    args.date = QString();
    args.dateFormat = QString();
    args.comment = QString();

    args.printFormat = defaultOutputFormat;
    args.lineNumber = -1;

    args.fileName = Settings::getLastFileName();

    args.reverse = false;
}

bool PodicepsCommandLine::writeToFile()
{
    QFile xmlFile(args.fileName);
    if(!xmlFile.open(QFile::WriteOnly | QFile::Text)) {
        errorMessage =  tr("Unable to open file %1").arg(args.fileName);
        return false;
    };

    QTextStream xmlContent(&xmlFile);
    QDomDocument document;
    QDomElement root = document.createElement("Words");
    document.appendChild(root);

    for(WordLine word : words) {
        QDomElement domElement = document.createElement("word");
        word.setDomElement(domElement);
        root.appendChild(domElement);
    }

    xmlContent << document.toString();
    xmlFile.close();

    return true;
}

void PodicepsCommandLine::addWord(bool *ok)
{
    setArguments();

    if(args.original.isEmpty()) {
        *ok = false;
        errorMessage = tr("The original word isn't specified.");
        return;
    }

    if(!setWords()) {
        *ok = false;
        return;
    }

    WordLine word(args.original, args.translation);

    if(words.contains(word)) {
        *ok = false;
        errorMessage = tr("The word already exists");
        return;
    }

    word.setTranscription(args.transcription);
    word.setStatusByTranslation(args.status);
    word.setCurrentDate();
    word.setComment(args.comment);

    words.append(word);

    *ok = writeToFile() ? true : false;
}

void PodicepsCommandLine::deleteWord(bool *ok)
{
    if(args.original.isEmpty()) {
        *ok = false;
        errorMessage = tr("The original word isn't specified.");
        return;
    }

    if(!setWords()) {
        *ok = false;
        return;
    }

    WordLine word(args.original);

    if(!words.removeOne(word)) {
        *ok = false;
        errorMessage = tr("The word doesn't exist");
        return;
    }

    *ok = writeToFile() ? true : false;
}

void PodicepsCommandLine::editWord(bool *ok)
{
    if(args.oldOriginal.isEmpty()) {
        *ok = false;
        errorMessage = tr("The original word isn't specified.");
        return;
    }

    if(!setWords()) {
        *ok = false;
        return;
    }

    WordLine word(args.oldOriginal);
    qsizetype index = words.lastIndexOf(word);
    if(index == -1) {
        *ok = false;
        errorMessage = tr("The word \"%1\" doesn't exist").arg(args.oldOriginal);
        return;
    }
    else {
        word = words[index];
        words.removeAt(index);
    }

    if(parser->isSet("o")) {
        QString original = parser->value("o");

        if(original.isEmpty()) {
            *ok = false;
            errorMessage = tr("The original word isn't specified.");
            return;
        }

        word.setOriginal(original);

        if(words.contains(word)) {
            *ok = false;
            errorMessage = tr("The word \"%1\" already exists").arg(original);
            return;
        }
    }

    if(parser->isSet("T")) {
        word.setTranscription(parser->value("T"));
    }

    if(parser->isSet("t")) {
        word.setTranslation(parser->value("t"));
    }

    if(parser->isSet("S")) {
        word.setStatusByTranslation(parser->value("S"));
    }

    if(parser->isSet("C")) {
        word.setComment(parser->value("C"));
    }

    words.append(word);

    *ok = writeToFile() ? true : false;
}

void PodicepsCommandLine::printWords(bool *ok)
{
    if(!setWords()) {
        *ok = false;
        return;
    }

    setArguments();

    QList<WordLine> printableWords;
    setPrintableWords(printableWords);
    if(printableWords.isEmpty()) {
        errorMessage = tr("No words matching the specified filters were found");
        *ok = false;
        return;
    }

    if(args.reverse) {
        std::reverse(printableWords.begin(), printableWords.end());
    }

    if(args.lineNumber != -1) {
        int endPos = args.lineNumber;
        if(endPos < 0 || endPos >= printableWords.size()) {
            endPos = printableWords.size();
        }

        while(printableWords.size() > endPos) {
            printableWords.pop_back();
        }
    }

    MaxLength maxLen;
    setMaxLength(printableWords, maxLen);

    QList<QString> finalStrings;
    setPrintStrings(printableWords, finalStrings, maxLen);

    for(const QString &s : finalStrings) {
        std::cout << s.toStdString();

        if(s != finalStrings.last()) {
            std::cout << "\n";
        }
    }
}

void PodicepsCommandLine::setPrintableWords(QList<WordLine> &rWords) const
{
    if(!args.original.isEmpty()) {
        for(const WordLine &w : words) {
            if(w.getOriginal().contains(args.original)) {
                rWords.append(w);
            }
        }
    }
    else {
        rWords = words;
    }

    if(!args.transcription.isEmpty()) {
        QList<WordLine> tmp;
        for(const WordLine &w : rWords) {
            if(w.getTranscription().contains(args.transcription)) {
                tmp.append(w);
            }
        }
        rWords.swap(tmp);
    }

    if(!args.translation.isEmpty()) {
        QList<WordLine> tmp;
        for(const WordLine &w : rWords) {
            if(w.getTranslation().contains(args.translation)) {
                tmp.append(w);
            }
        }
        rWords.swap(tmp);
    }

    if(!args.status.isEmpty()) {
        QList<WordLine> tmp;
        for(const WordLine &w : rWords) {
            if(w.getStatusTranslation().contains(args.status)) {
                tmp.append(w);
            }
        }
        rWords.swap(tmp);
    }

    if(!args.date.isEmpty()) {
        QList<WordLine> tmp;
        QString format = args.dateFormat;
        for(const WordLine &w : rWords) {
            if(w.getDate(format).contains(args.date)) {
                tmp.append(w);
            }
        }
        rWords.swap(tmp);
    }

    if(!args.comment.isEmpty()) {
        QList<WordLine> tmp;
        for(const WordLine &w : rWords) {
            if(w.getComment().contains(args.comment)) {
                tmp.append(w);
            }
        }
        rWords.swap(tmp);
    }
}

void PodicepsCommandLine::setMaxLength(const QList<WordLine> &pWords,
                                       MaxLength &len) const
{
    len.original = 0;
    len.transcription = 0;
    len.translation = 0;
    len.status = 0;
    len.date = 0;
    len.comment = 0;

    if(pWords.isEmpty()) {
        return;
    }

    for(const WordLine &w : pWords) {
        int orl = w.getOriginal().size();
        len.original = orl > len.original ? orl : len.original;

        int trp = w.getTranscription().size();
        len.transcription = trp > len.transcription ? trp : len.transcription;

        int trl = w.getTranslation().size();
        len.translation = trl > len.translation ? trl : len.translation;

        int sts = w.getStatus().size();
        len.status = sts > len.status ? sts : len.status;

        int dt = w.getDate().size();
        len.date = dt > len.date ? dt : len.date;

        int cmt = w.getComment().size();
        len.comment = cmt > len.comment ? cmt : len.comment;
    }
}

void PodicepsCommandLine::setPrintStrings(const QList<WordLine> &pWords,
                                          QList<QString> &rStrings,
                                          const MaxLength &len) const
{
    if(pWords.isEmpty()) {
        qDebug() << "Passed an empty argument";
        return;
    }

    bool printOriginal = args.printFormat.contains("o");
    bool printTranscription = args.printFormat.contains("T");
    bool printTranslation = args.printFormat.contains("t");
    bool printStatus = args.printFormat.contains("S");
    bool printDate = args.printFormat.contains("D");
    bool printComment = args.printFormat.contains("C");

    int spanSize = 2;
    for(const WordLine &w : pWords) {
        QString fullString;

        if(printOriginal) {
            QString original = w.getOriginal();
            int spaces = len.original - original.size() + spanSize;
            fullString += original + QString(spaces, ' ');
        }

        if(printTranscription) {
            QString transcription = w.getTranscription();
            int spaces = len.transcription - transcription.size() + spanSize;
            fullString += transcription + QString(spaces, ' ');
        }

        if(printTranslation) {
            QString translation = w.getTranslation();
            int spaces = len.translation - translation.size() + spanSize;
            fullString += translation + QString(spaces, ' ');
        }

        if(printStatus) {
            QString status = w.getStatusTranslation();
            int spaces = len.status - status.size() + spanSize;
            fullString += status + QString(spaces, ' ');
        }

        if(printDate) {
            QString date = w.getDate(args.dateFormat);
            int spaces = len.date - date.size() + spanSize;
            fullString += date + QString(spaces, ' ');
        }

        if(printComment) {
            QString comment = w.getComment();
            int spaces = len.comment - comment.size() + spanSize;
            fullString += comment + QString(spaces, ' ');
        }

        if(!fullString.isEmpty()) {
            rStrings.append(fullString);
        }
    }
}

bool PodicepsCommandLine::isValidFormat(const QString &format) const
{
    // Default format.
    if(format.isEmpty()) {
        return true;
    }

    for(int i = 0; i < format.size(); ++i) {
        if(!defaultOutputFormat.contains(format[i])) {
            return false;
        }
    }

    return true;
}

PodicepsCommandLine::PodicepsCommandLine(int &argc, char **argv)
    : QCoreApplication(argc, argv), parser(new QCommandLineParser),
      errorMessage(QString())
{
    setLanguage();
    setArgumentsDefault();

    setApplicationName("podiceps");
    setApplicationVersion(PODICEPS_VERSION);

    setupParser();
}

PodicepsCommandLine::~PodicepsCommandLine()
{

}
