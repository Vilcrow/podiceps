/**************************************************************************/
/*  manual_widget.cpp                                                     */
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

#include "manual_widget.h"
#include "text_file.h"
#include <QDebug>
#include <QPushButton>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>

void ManualWidget::setupTabs()
{
    if(!tabWidget) {
        qDebug() << "Passed a null pointer";
        return;
    }

    for(Tabs t : tabs) {
        QWidget *tab = new QWidget(tabWidget);
        setupTextEdit(tab, tabContentPaths[t]);
        tabWidget->addTab(tab, tabNames[t]);
    }
}

void ManualWidget::setupTextEdit(QWidget *widget, const QString &path)
{
    if(!tabWidget || !widget) {
        qDebug() << "Passed a null pointer";
        return;
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);
    QTextEdit *textEdit = new QTextEdit(widget);
    QString text = TextFile::getFileContent(path);
    textEdit->setHtml(text);
    textEdit->setReadOnly(true);
    mainLayout->addWidget(textEdit);
}

ManualWidget::ManualWidget(QWidget *parent)
    : QDialog(parent), tabWidget(new QTabWidget(this))
{
    tabs = {General, Dictionary, Preferences, Shortcuts};

    tabNames = {{General, tr("General")},
                {Dictionary, tr("Dictionary")},
                {Preferences, tr("Preferences")},
                {Shortcuts, tr("Shortcuts")}};

    tabContentPaths = {{General, ":/general.html"},    
                       {Dictionary, ":/dictionary.html"},
                       {Preferences, ":/preferences.html"},
                       {Shortcuts, ":/shortcuts.html"}};

    setupTabs();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(100);
    connect(closeButton, &QAbstractButton::clicked, this, &QDialog::accept);

    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);

    setWindowTitle(tr("User Manual"));
    setMinimumSize(600, 500);
}

ManualWidget::~ManualWidget()
{

}
