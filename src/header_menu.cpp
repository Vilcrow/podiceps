/**************************************************************************/
/*  header_menu.cpp                                                       */
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

#include "header_menu.h"
#include "settings.h"

void HeaderMenu::updateSettings(bool checked)
{
    QAction *action = static_cast<QAction*>(sender());
    if(action == transcriptionAct) {
        showTranscription = checked;
    }
    else if(action == statusAct) {
        showStatus = checked;
    }
    else if(action == dateAct) {
        showDate = checked;
    }

    writeSettings();
}

void HeaderMenu::readSettings()
{
    showTranscription = Settings::isTranscriptionVisible();
    showStatus = Settings::isStatusVisible();
    showDate = Settings::isDateVisible();
}

void HeaderMenu::writeSettings()
{
    Settings::setTranscriptionVisible(showTranscription);
    Settings::setStatusVisible(showStatus);
    Settings::setDateVisible(showDate);
}

HeaderMenu::HeaderMenu(QWidget *parent)
    : QMenu(parent)
{
    readSettings();

    transcriptionAct = new QAction(tr("Transcription"), this);
    transcriptionAct->setCheckable(true);
    transcriptionAct->setChecked(showTranscription);
    addAction(transcriptionAct);

    statusAct = new QAction(tr("Status"), this);
    statusAct->setCheckable(true);
    statusAct->setChecked(showStatus);
    addAction(statusAct);

    dateAct = new QAction(tr("Date"), this);
    dateAct->setCheckable(true);
    dateAct->setChecked(showDate);
    addAction(dateAct);

    addSeparator();

    alignAct = new QAction(tr("Align"), this);
    addAction(alignAct);

    connect(transcriptionAct, &QAction::toggled,
            this, &HeaderMenu::updateSettings);
    connect(statusAct, &QAction::toggled,
            this, &HeaderMenu::updateSettings);
    connect(dateAct, &QAction::toggled,
            this, &HeaderMenu::updateSettings);
    connect(alignAct, &QAction::triggered,
            this, &HeaderMenu::updateSettings);
}

HeaderMenu::~HeaderMenu()
{

}
