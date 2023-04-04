/**************************************************************************/
/*  settings.cpp                                                          */
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

#include "settings.h"
#include "text_file.h"

QSettings Settings::settings("Vilcrow", "podiceps");

QMap<int, QString> Settings::themePaths = { {LightTheme, ""},
                                            {DarkTheme, ""},
                                            {CustomTheme, ""} };

QMap<int, QString> Settings::languages = { {EnglishLang, "English"},
                                           {RussianLang, "Русский"}};

QMap<int, QString> Settings::langPrefixes = { {EnglishLang, ":/"},
                                              {RussianLang, ":/ru/"}};

Settings::Language Settings::getLanguage()
{
    settings.beginGroup("/Settings/Interface");
    int appLang = settings.value("/app_language", 0).toInt();
    settings.endGroup();

    Language ret = EnglishLang;

    switch(appLang) {
    case EnglishLang:
        break;
    case RussianLang:
        ret = RussianLang;
        break;
    default:
        break;
    }

    return ret;
}

QString Settings::getLanguageName(Language lang)
{
    return languages[lang];
}

QString Settings::getLanguagePrefix(Language lang)
{
    return langPrefixes[lang];
}

Settings::Theme Settings::getTheme()
{
    Theme ret = LightTheme;

    settings.beginGroup("/Settings/Interface");
    int theme = settings.value("/app_theme", 0).toInt();
    settings.endGroup();

    switch(theme) {
    case LightTheme:
        ret = LightTheme;
        break;
    case DarkTheme:
        ret = DarkTheme;
        break;
    case CustomTheme:
        ret = CustomTheme;
        break;
    default:
        break;
    }

    return ret;
}

QString Settings::getThemeText()
{
    setThemePaths();

    QString ret = "";
    QString path = "";

    int theme = getTheme();

    switch(theme) {
    case LightTheme:
        path = themePaths[LightTheme];
        break;
    case DarkTheme:
        path = themePaths[DarkTheme];
        break;
    case CustomTheme:
        path = themePaths[CustomTheme];
        break;
    }

    // If not the default theme.
    if(!path.isEmpty()) {
        ret = TextFile::getFileContent(path);
    }

    return ret;
}

QString Settings::getCustomThemePath()
{
    settings.beginGroup("/Settings/Interface");
    QString ret = settings.value("/custom_theme_path", "").toString();
    settings.endGroup();

    return ret;
}

bool Settings::isTranscriptionVisible()
{
    settings.beginGroup("/Settings/Interface");
    bool ret = settings.value("/table/show_transcription", true).toBool();
    settings.endGroup();

    return ret;
}

bool Settings::isStatusVisible()
{
    settings.beginGroup("/Settings/Interface");
    bool ret = settings.value("/table/show_status", true).toBool();
    settings.endGroup();

    return ret;
}

bool Settings::isDateVisible()
{
    settings.beginGroup("/Settings/Interface");
    bool ret = settings.value("/table/show_date", true).toBool();
    settings.endGroup();

    return ret;
}

QString Settings::getLastFileName()
{
    settings.beginGroup("/Settings");
    QString ret = settings.value("/last_file_name", "").toString();
    settings.endGroup();

    return ret;
}

void Settings::setTheme(const Theme theme)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/app_theme", theme);
    settings.endGroup();
}

void Settings::setCustomThemePath(const QString &path)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/custom_theme_path", path);
    settings.endGroup();
}

void Settings:: setLanguage(const Language lang)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/app_language", lang);
    settings.endGroup();
}

void Settings::setTranscriptionVisible(bool visible)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/table/show_transcription", visible);
    settings.endGroup();
}

void Settings::setStatusVisible(bool visible)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/table/show_status", visible);
    settings.endGroup();
}

void Settings::setDateVisible(bool visible)
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/table/show_date", visible);
    settings.endGroup();
}

void Settings::setLastFileName(const QString &name)
{
    settings.beginGroup("/Settings");
    settings.setValue("last_file_name", name);
    settings.endGroup();
}

void Settings::Settings::setThemePaths()
{
    // Default theme(Qt).
    themePaths[LightTheme] = "";

    themePaths[DarkTheme] = ":/theme_dark.qss";

    settings.beginGroup("/Settings/Interface");
    themePaths[CustomTheme] = settings.value("/custom_theme_path", "").toString();
    settings.endGroup();
}


Settings::Settings()
{

}

Settings::~Settings()
{

}
