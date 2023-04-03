/**************************************************************************/
/*  settings.h                                                            */
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

#ifndef SETTINGS_VIL_H
#define SETTINGS_VIL_H

#include <QSettings>

class Settings {
public:
    enum Theme { LightTheme, DarkTheme, CustomTheme };
    enum Language { EnglishLang, RussianLang };

    static Theme getTheme();
    static QString getThemeText();
    static QString getCustomThemePath();
    static Language getLanguage();
    static QString getLanguageName(Language lang);

    static bool isTranscriptionVisible();
    static bool isStatusVisible();
    static bool isDateVisible();

    static QString getLastFileName();

    static void setTheme(const Theme theme);
    static void setCustomThemePath(const QString &path);
    static void setLanguage(const Language lang);

    static void setTranscriptionVisible(bool visible);
    static void setStatusVisible(bool visible);
    static void setDateVisible(bool visible);

    static void setLastFileName(const QString &name);
private:
    Settings();
    ~Settings();

    static QSettings settings;
    static QMap<int, QString> themePaths;
    static QMap<int, QString> languages;

    static void setThemePaths();
};

#endif
