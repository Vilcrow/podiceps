/**************************************************************************/
/*  file_info_widget.cpp                                                  */
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

#include "file_info_widget.h"
#include <QDateTime>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>

FileInfoWidget::FileInfoWidget(const QString &name, QWidget *parent)
    : QDialog(parent), fileInfo(name)
{
    setWindowTitle(tr("File Info"));

    QGridLayout *mainLayout = new QGridLayout;

    QLabel *nameLabel = new QLabel(tr("Name:"));
    QLabel *pathLabel = new QLabel(tr("Path:"));
    QLabel *dateLabel = new QLabel(tr("Date:"));
    QLabel *timeLabel = new QLabel(tr("Time:"));

    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(pathLabel, 1, 0);
    mainLayout->addWidget(dateLabel, 2, 0);
    mainLayout->addWidget(timeLabel, 3, 0);

    QLabel *nameValueLabel = new QLabel(fileInfo.fileName());
    QLabel *pathValueLabel = new QLabel(fileInfo.filePath());

    QDateTime lastModifiedTime = fileInfo.lastModified();
    QString date = lastModifiedTime.toString("yyyy-MM-dd");
    QString time = lastModifiedTime.toString("HH:mm:ss");
    QLabel *dateValueLabel = new QLabel(date);
    QLabel *timeValueLabel = new QLabel(time);

    mainLayout->addWidget(nameValueLabel, 0, 1);
    mainLayout->addWidget(pathValueLabel, 1, 1);
    mainLayout->addWidget(dateValueLabel, 2, 1);
    mainLayout->addWidget(timeValueLabel, 3, 1);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QAbstractButton::clicked, this, &QDialog::accept);

    mainLayout->addWidget(closeButton, 4, 1);

    setLayout(mainLayout);
}

FileInfoWidget::~FileInfoWidget()
{

}
