/**************************************************************************/
/*  about_widget.cpp                                                      */
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

#include "about_widget.h"
#include "preferences_widget.h"
#include <QDebug>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

QString AboutWidget::getFullLicenseText()
{
    return getFileContent(":/license_gpl_v3.txt");
}

QString AboutWidget::getShortLicenseText()
{
    QSettings settings("Vilcrow", "podiceps");
    settings.beginGroup("/Settings/Interface");
    int appLanguage = settings.value("/app_language", 0).toInt();
    settings.endGroup();

    QString path;
    switch(appLanguage) {
    case PreferencesWidget::RussianLang:
        path = ":/license_short_ru.txt";
        break;
    default:  // Default English language.
        path = ":/license_short_en.txt";
        break;
    }

    return getFileContent(path);
}

QString AboutWidget::getFileContent(const QString &path)
{
    QString ret;

    if(path.isEmpty()) {
        qDebug() << "Passed an empty argument";
        return ret;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream content(&file);
        ret = content.readAll();
    }
    else {
        qDebug() << QString("Unable to open the license file: %1").arg(path);
    }

    return ret;
}

void AboutWidget::openLicense()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QDialog *licenseDialog = new QDialog(this);
    licenseDialog->setFixedSize(500, 250);

    QString licenseText = getShortLicenseText();

    QTextEdit *textEdit = new QTextEdit(licenseDialog);
    textEdit->setText(licenseText);
    textEdit->setReadOnly(true);
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    mainLayout->addWidget(textEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonLayout->addWidget(closeButton);

    QPushButton *fullTextButton = new QPushButton("GPLv3");
    buttonLayout->addWidget(fullTextButton);

    mainLayout->addLayout(buttonLayout);
    licenseDialog->setLayout(mainLayout);

    licenseDialog->setWindowTitle(tr("License"));

    connect(closeButton, &QAbstractButton::clicked,
            licenseDialog, &QDialog::accept);
    connect(fullTextButton, &QAbstractButton::clicked,
            this, &AboutWidget::openFullText);

    licenseDialog->exec();
}

void AboutWidget::openFullText()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QDialog *licenseDialog = new QDialog(this);
    licenseDialog->setMinimumSize(500, 300);

    QString licenseText = getFullLicenseText();

    QTextEdit *textEdit = new QTextEdit(licenseDialog);
    textEdit->setText(licenseText);
    textEdit->setReadOnly(true);
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    mainLayout->addWidget(textEdit);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(100);
    mainLayout->addWidget(closeButton);

    licenseDialog->setLayout(mainLayout);

    licenseDialog->setWindowTitle("GPLv3");

    connect(closeButton, &QAbstractButton::clicked,
            licenseDialog, &QDialog::accept);

    licenseDialog->exec();
}

AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QString nameAndVersion = "podiceps " + QString(PODICEPS_VERSION);
    QLabel *nameLabel = new QLabel(nameAndVersion);
    nameLabel->setAlignment(Qt::AlignHCenter);
    nameLabel->setStyleSheet("font-weight: bold");
    mainLayout->addWidget(nameLabel);

    QLabel *descriptionLabel = new QLabel(tr("A simple program for maintaining "
                                             "a dictionary of foreign words."));
    descriptionLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(descriptionLabel);

    QString copyrightChar = QChar(0x00A9);
    QString copyright = tr("Copyright") + copyrightChar +
                           " 2022-2023";
    QLabel *copyrightLabel = new QLabel(copyright);
    copyrightLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(copyrightLabel);

    QLabel *authorLabel = new QLabel("Vilnur Shakirov");
    authorLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(authorLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *licenseButton = new QPushButton(tr("License"));
    licenseButton->setFixedWidth(100);
    buttonLayout->addWidget(licenseButton);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(100);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("About"));

    connect(licenseButton, &QAbstractButton::clicked,
            this, &AboutWidget::openLicense);
    connect(closeButton, &QAbstractButton::clicked,
            this, &QDialog::accept);
}

AboutWidget::~AboutWidget()
{

}
