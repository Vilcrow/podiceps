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
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTextStream>
#include <QVBoxLayout>

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
        customThemePath = path;
        customLineEdit->setText(path);
    }
}

void PreferencesWidget::languageChanged(int index)
{
    langHintLabel->show();

    switch(index) {
    case Settings::EnglishLang:
        appLanguage = Settings::EnglishLang;
        break;
    case Settings::RussianLang:
        appLanguage = Settings::RussianLang;
        break;
    default:
        appLanguage = Settings::EnglishLang;
        break;
    }
}

void PreferencesWidget::themeChanged(bool checked)
{
    if(checked) {
        QRadioButton *button = static_cast<QRadioButton*>(sender());
        if(button == lightThemeButton) {
            appTheme = Settings::LightTheme;
        }
        else if(button == darkThemeButton) {
            appTheme = Settings::DarkTheme;
        }
        else {
            appTheme = Settings::CustomTheme;
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
    appLanguage = Settings::getLanguage();
    appTheme = Settings::getTheme();
    customThemePath = Settings::getCustomThemePath();
    showTranscription = Settings::isTranscriptionVisible();
    showStatus = Settings::isStatusVisible();
    showDate = Settings::isDateVisible();
}

void PreferencesWidget::writeSettings()
{
    Settings::setLanguage(appLanguage);
    Settings::setTheme(appTheme);
    Settings::setCustomThemePath(customLineEdit->text());
    Settings::setTranscriptionVisible(showTranscription);
    Settings::setStatusVisible(showStatus);
    Settings::setDateVisible(showDate);
}

void PreferencesWidget::setupInterfaceTab()
{
    if(!tabWidget) {
        qDebug() << "Passed a null pointer";
        return;
    }

    QWidget *interfaceTab = new QWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    interfaceTab->setLayout(mainLayout);
    tabWidget->addTab(interfaceTab, tr("&Interface"));

    // Language settings.
    QGroupBox *langGroupBox = new QGroupBox(tr("Language"));
    QVBoxLayout *langLayout = new QVBoxLayout;

    languageBox = new QComboBox(this);
    languageBox->addItem(Settings::getLanguageName(Settings::EnglishLang));
    languageBox->addItem(Settings::getLanguageName(Settings::RussianLang));
    languageBox->setCurrentIndex(appLanguage);
    langLayout->addWidget(languageBox);
    connect(languageBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(languageChanged(int)));

    langHintLabel = new QLabel(tr("*restart the program to apply the changes"));
    langHintLabel->hide();
    langLayout->addWidget(langHintLabel);

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
    customLineEdit = new QLineEdit(customThemePath);
    customLineEdit->setReadOnly(true);
    customLayout->addWidget(customLineEdit);
    QPushButton *customChooseButton = new QPushButton(tr("Ch&oose..."));
    customLayout->addWidget(customChooseButton);
    connect(customChooseButton, &QAbstractButton::clicked,
            this, &PreferencesWidget::setCustomThemePath);

    switch(appTheme) {
    case Settings::LightTheme:
        lightThemeButton->setChecked(true);
        break;
    case Settings::DarkTheme:
        darkThemeButton->setChecked(true);
        break;
    case Settings::CustomTheme:
        customThemeButton->setChecked(true);
        break;
    }

    themeLayout->addLayout(customLayout);
    themeGroupBox->setLayout(themeLayout);
    mainLayout->addWidget(themeGroupBox);
}

void PreferencesWidget::setupTableTab()
{
    if(!tabWidget) {
        qDebug() << "Passed a null pointer";
        return;
    }

    QWidget *tableTab = new QWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    tableTab->setLayout(mainLayout);
    tabWidget->addTab(tableTab, tr("&Table"));

    QGroupBox *tableGroupBox = new QGroupBox(tr("Table View"));
    QVBoxLayout *tableLayout = new QVBoxLayout;

    showTranscriptionCheckBox = new QCheckBox(tr("T&ranscription"), tableTab);
    showTranscriptionCheckBox->setChecked(showTranscription);
    tableLayout->addWidget(showTranscriptionCheckBox);
    connect(showTranscriptionCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    showStatusCheckBox = new QCheckBox(tr("Stat&us"), tableTab);
    showStatusCheckBox->setChecked(showStatus);
    tableLayout->addWidget(showStatusCheckBox);
    connect(showStatusCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    showDateCheckBox = new QCheckBox(tr("&Date"), tableTab);
    showDateCheckBox->setChecked(showDate);
    tableLayout->addWidget(showDateCheckBox);
    connect(showDateCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::tableSettingsChanged);

    tableGroupBox->setLayout(tableLayout);
    mainLayout->addWidget(tableGroupBox);
}

PreferencesWidget::PreferencesWidget(QWidget *parent)
    : QDialog(parent), tabWidget(new QTabWidget(this))
{
    readSettings();
    setupInterfaceTab();
    setupTableTab();

    setWindowTitle(tr("Preferences"));
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tabWidget);

    QPushButton *saveButton = new QPushButton(tr("&Save"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
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
