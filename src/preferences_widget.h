/**************************************************************************/
/*  preferences_widget.h                                                  */
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

#ifndef PREFERENCES_WIDGET_VIL_H
#define PREFERENCES_WIDGET_VIL_H

#include <QDialog>
#include <QSettings>
#include <QTabWidget>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QRadioButton;

class PreferencesWidget : public QDialog {
    Q_OBJECT
public:
    enum Theme { LightTheme, DarkTheme, CustomTheme };

    static QString getTheme(int theme);

    enum Language { EnglishLang, RussianLang };

    PreferencesWidget(QWidget *parent = nullptr);
    virtual ~PreferencesWidget();
public slots:
    void accept() override;
private slots:
    void setCustomThemePath();
    void languageChanged(int index);
    void themeChanged(bool checked);
    void tableSettingsChanged(int state);
private:
    QSettings settings;
    int appLanguage;
    int appTheme;
    bool showTranscription;
    bool showStatus;
    bool showDate;

    QTabWidget *tabWidget;

    QComboBox *languageBox;
    QLabel *langHintLabel;

    QCheckBox *showTranscriptionCheckBox;
    QCheckBox *showStatusCheckBox;
    QCheckBox *showDateCheckBox;

    QRadioButton *lightThemeButton;
    QRadioButton *darkThemeButton;
    QRadioButton *customThemeButton;
    QLineEdit *customLineEdit;

    static QMap<int, QString> themePaths;
    static void setThemePaths();

    static QMap<int, QString> languages;

    void readSettings();
    void writeSettings();
    void setupInterfaceTab();
    void setupTableTab();
};

#endif
