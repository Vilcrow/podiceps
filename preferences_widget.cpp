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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

void PreferencesWidget::setCustomThemePath()
{
    QString path = QFileDialog::getOpenFileName(this);
    if(!path.isEmpty()) {
        customThemePath = path;
        customLineEdit->setText(path);
    }
}

void PreferencesWidget::onToggled(bool checked)
{
    if(checked) {
        QRadioButton *b = static_cast<QRadioButton*>(sender());
        if(b == lightThemeButton) {
            appTheme = LightTheme;
        }
        else if(b == darkThemeButton) {
            appTheme = DarkTheme;
        }
        else {
            appTheme = CustomTheme;
        }
    }
}

void PreferencesWidget::stateChanged(int state)
{
    QCheckBox *b = static_cast<QCheckBox*>(sender());
    if(b == showStatusCheckBox) {
        if(state == Qt::Unchecked) {
            showStatus = false;
        }
        else if(state == Qt::Checked) {
            showStatus = true;
        }
    }
    else if(b == showDateCheckBox) {
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
    settings.beginGroup("/Settings/Inteface");
    appTheme = settings.value("/app_theme", 0).toInt();
    customThemePath = settings.value("/custom_theme_path", "").toString();
    showStatus = settings.value("/table/show_status", true).toBool();
    showDate = settings.value("/table/show_date", true).toBool();
    settings.endGroup();
}

void PreferencesWidget::writeSettings()
{
    settings.beginGroup("/Settings/Inteface");
    settings.setValue("/app_theme", appTheme);
    settings.setValue("/custom_theme_path", customLineEdit->text());
    settings.setValue("/table/show_status", showStatus);
    settings.setValue("/table/show_date", showDate);
    settings.endGroup();
}

void PreferencesWidget::setupIntefaceTab()
{
    if(!tabWidget) {
        return;
    }

    // Interface tab.
    QWidget *interfaceTab = new QWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    interfaceTab->setLayout(mainLayout);
    tabWidget->addTab(interfaceTab, tr("&Interface"));

    // Theme settings.
    QGroupBox *themeGroupBox = new QGroupBox(tr("Theme"));
    QVBoxLayout *themeLayout = new QVBoxLayout;

    lightThemeButton = new QRadioButton(tr("&Light"));
    themeLayout->addWidget(lightThemeButton);
    connect(lightThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::onToggled);

    darkThemeButton = new QRadioButton(tr("&Dark"));
    themeLayout->addWidget(darkThemeButton);
    connect(darkThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::onToggled);

    QHBoxLayout *customLayout = new QHBoxLayout;
    customThemeButton = new QRadioButton(tr("C&ustom"));
    customLayout->addWidget(customThemeButton);
    connect(customThemeButton, &QRadioButton::toggled,
            this, &PreferencesWidget::onToggled);
    customLineEdit = new QLineEdit(customThemePath);
    customLayout->addWidget(customLineEdit);
    QPushButton *customOpenButton = new QPushButton(tr("&Open"));
    customLayout->addWidget(customOpenButton);
    connect(customOpenButton, &QAbstractButton::clicked,
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

    showStatusCheckBox = new QCheckBox(tr("S&tatus column"), interfaceTab);
    showStatusCheckBox->setChecked(showStatus);
    tableLayout->addWidget(showStatusCheckBox);
    connect(showStatusCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::stateChanged);

    showDateCheckBox = new QCheckBox(tr("D&ate column"), interfaceTab);
    showDateCheckBox->setChecked(showDate);
    tableLayout->addWidget(showDateCheckBox);
    connect(showDateCheckBox, &QCheckBox::stateChanged,
            this, &PreferencesWidget::stateChanged);

    tableGroupBox->setLayout(tableLayout);
    mainLayout->addWidget(tableGroupBox);
}

PreferencesWidget::PreferencesWidget(QWidget *parent)
    : QDialog(parent), settings("Vilcrow", "podiceps")
{
    readSettings();
    tabWidget = new QTabWidget(this);
    setupIntefaceTab();

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

    connect(saveButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}

PreferencesWidget::~PreferencesWidget()
{
    if(result() == QDialog::Accepted) {
        writeSettings();
    }
}
