/**************************************************************************/
/*  save_dialog.cpp                                                       */
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

#include "save_dialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

void SaveDialog::cancel()
{
    cancelled = true;
    reject();
}

bool SaveDialog::isCancelled() const
{
    return cancelled;
}

int SaveDialog::trySave()
{
    cancelled = false;

    int result = exec();
    if(result == QDialog::Accepted) {
        return SaveDialog::Accepted;
    }
    else if(cancelled) {
        return SaveDialog::Cancelled;
    }
    else {
        return SaveDialog::Rejected;
    }
}

SaveDialog::SaveDialog(QWidget *parent)
    : QDialog(parent), cancelled(false)
{
    setWindowTitle(tr("Save changes"));
    QLabel *infoLabel = new QLabel(tr("Save changes to the current file?"));
    QPushButton *saveButton = new QPushButton(tr("Save"));
    QPushButton *noButton = new QPushButton(tr("No"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(infoLabel);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(saveButton);
    hLayout->addWidget(noButton);
    hLayout->addWidget(cancelButton);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(saveButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(noButton, &QAbstractButton::clicked, this, &QDialog::reject);
    connect(cancelButton, &QAbstractButton::clicked, this, &SaveDialog::cancel);
}

SaveDialog::~SaveDialog()
{

}
