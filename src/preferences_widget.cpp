/**************************************************************************/
/*  preferences_widget.cpp                                                */
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

#include "preferences_widget.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTextStream>
#include <QVBoxLayout>

QMap<int, QString> PreferencesWidget::themePaths = { {LightTheme, ""},
                                                     {DarkTheme, ""},
                                                     {CustomTheme, ""} };

void PreferencesWidget::setThemePaths()
{
    // Default theme(Qt).
    themePaths[LightTheme] = "";

    themePaths[DarkTheme] = "../../themes/theme_dark.qss";

    QSettings settings("Vilcrow", "podiceps");
    settings.beginGroup("/Settings/Interface");
    themePaths[CustomTheme] = settings.value("/custom_theme_path", "").toString();
    settings.endGroup();
}

QString PreferencesWidget::getTheme(int theme)
{
    setThemePaths();

    QString ret = "";
    QString path = "";

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

    if(!path.isEmpty()) {
        QFile themeFile(path);
        if(themeFile.open(QIODevice::ReadOnly)) {
            QTextStream content(&themeFile);
            ret = content.readAll();
        }
    }

    return ret;
}

void PreferencesWidget::accept()
{
    writeSettings();
    QDialog::accept();
}

void PreferencesWidget::setCustomThemePath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                         "", tr("QSS files (*.qss)"));
    if(!path.isEmpty()) {
        themePaths[CustomTheme] = path;
        customLineEdit->setText(path);
    }
}

void PreferencesWidget::languageChanged(bool checked)
{
    if(checked) {
        QRadioButton *button = static_cast<QRadioButton*>(sender());
        if(button == engLangButton) {
            appLanguage = EnglishLang;
        }
    }
}

void PreferencesWidget::themeChanged(bool checked)
{
    if(checked) {
        QRadioButton *button = static_cast<QRadioButton*>(sender());
        if(button == lightThemeButton) {
            appTheme = LightTheme;
        }
        else if(button == darkThemeButton) {
            appTheme = DarkTheme;
        }
        else {
            appTheme = CustomTheme;
        }
    }
}

void PreferencesWidget::tableSettingsChanged(int state)
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(sender());
    if(checkBox == showTranscriptionCheckBox) {
        if(state == Qt::Unchecked) {
            showTranscription = false;
        }
        else if(state == Qt::Checked) {
            showTranscription = true;
        }
    }
    else if(checkBox == showStatusCheckBox) {
        if(state == Qt::Unchecked) {
            showStatus = false;
        }
        else if(state == Qt::Checked) {
            showStatus = true;
        }
    }
    else if(checkBox == showDateCheckBox) {
        if(state == Qt::Unchecked) {
            showDate = false;
        }
        else if(state == Qt::Checked) {
            showDate = true;
        }
    }
}

void PreferencesWidget::readSettings()
{
    settings.beginGroup("/Settings/Interface");
    appLanguage = settings.value("/app_language", 0).toInt();
    appTheme = settings.value("/app_theme", 0).toInt();
    themePaths[CustomTheme] = settings.value("/custom_theme_path", "").toString();
    showTranscription = settings.value("/table/show_transcription", true).toBool();
    showStatus = settings.value("/table/show_status", true).toBool();
    showDate = settings.value("/table/show_date", true).toBool();
    settings.endGroup();
}

void PreferencesWidget::writeSettings()
{
    settings.beginGroup("/Settings/Interface");
    settings.setValue("/app_language", appLanguage);
    settings.setValue("/app_theme", appTheme);
    settings.setValue("/custom_theme_path", customLineEdit->text());
    settings.setValue("/table/show_transcription", showTranscription);
    settings.setValue("/table/show_status", showStatus);
    settings.setValue("/table/show_date", showDate);
    settings.endGroup();
}

void PreferencesWidget::setupInterfaceTab()
{
    if(!tabWidget) {
        return;
    }

    // Interface tab.
    QWidget *interfaceTab = new QWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    interfaceTab->setLayout(mainLayout);
    tabWidget->addTab(interfaceTab, tr("&Interface"));

    // Language settings.
    QGroupBox *langGroupBox = new QGroupBox(tr("Language"));
    QVBoxLayout *langLayout = new QVBoxLayout;
    engLangButton = new QRadioButton("English");
    langLayout->addWidget(engLangButton);
    connect(engLangButton, &QRadioButton::toggled,
            this, &PreferencesWidget::languageChanged);

    switch(appLanguage) {
    case EnglishLang:
        engLangButton->setChecked(true);
        break;
    }

    langGroupBox->setLayout(langLayout);
    mainLayout->addWidget(langGroupBox);

    // Theme settings.
    QGroupBox *themeGroupBox = new QGroupBox(tr("Theme"));
    QVBoxLayout *themeLayout = new QVBoxLayout;

    lightThemeButton = new QRadioButton(tr("&Light"));
    themeLayout->addWidget(lightThemeButton);
    connect(lightThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::themeChanged);

    darkThemeButton = new QRadioButton(tr("&Dark"));
    themeLayout->addWidget(darkThemeButton);
    connect(darkThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::themeChanged);

    QHBoxLayout *customLayout = new QHBoxLayout;
    customThemeButton = new QRadioButton(tr("C&ustom"));
    customLayout->addWidget(customThemeButton);
    connect(customThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::themeChanged);
    customLineEdit = new QLineEdit(themePaths[CustomTheme]);
    customLayout->addWidget(customLineEdit);
    QPushButton *customChooseButton = new QPushButton(tr("Ch&oose..."));
    customLayout->addWidget(customChooseButton);
    connect(customChooseButton, &QAbstractButton::clicked,
            this, &PreferencesWidget::setCustomThemePath);

    switch(appTheme) {
    case LightTheme:
        lightThemeButton->setChecked(true);
        break;
    case DarkTheme:
        darkThemeButton->setChecked(true);
        break;
    case CustomTheme:
        customThemeButton->setChecked(true);
        break;
    }

    themeLayout->addLayout(customLayout);
    themeGroupBox->setLayout(themeLayout);
    mainLayout->addWidget(themeGroupBox);

    // Table settings.
    QGroupBox *tableGroupBox = new QGroupBox(tr("Table"));
    QVBoxLayout *tableLayout = new QVBoxLayout;

    showTranscriptionCheckBox = new QCheckBox(tr("&Transcription column"), interfaceTab);
    showTranscriptionCheckBox->setChecked(showTranscription);
    tableLayout->addWidget(showTranscriptionCheckBox);
    connect(showTranscriptionCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    showStatusCheckBox = new QCheckBox(tr("S&tatus column"), interfaceTab);
    showStatusCheckBox->setChecked(showStatus);
    tableLayout->addWidget(showStatusCheckBox);
    connect(showStatusCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    showDateCheckBox = new QCheckBox(tr("D&ate column"), interfaceTab);
    showDateCheckBox->setChecked(showDate);
    tableLayout->addWidget(showDateCheckBox);
    connect(showDateCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    tableGroupBox->setLayout(tableLayout);
    mainLayout->addWidget(tableGroupBox);
}

PreferencesWidget::PreferencesWidget(QWidget *parent)
    : QDialog(parent), settings("Vilcrow", "podiceps")
{
    readSettings();
    tabWidget = new QTabWidget(this);
    setupInterfaceTab();

    setWindowTitle(tr("Preferences"));
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tabWidget);

    QPushButton *saveButton = new QPushButton(tr("&Save"));
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(saveButton);
    hLayout->addWidget(cancelButton);

    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(saveButton, &QAbstractButton::clicked,
            this, &PreferencesWidget::accept);
    connect(cancelButton, &QAbstractButton::clicked,
            this, &QDialog::reject);
}

PreferencesWidget::~PreferencesWidget()
{

}
