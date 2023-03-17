/**************************************************************************/
/*  action_log.cpp                                                        */
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

#include "action_log.h"

bool ActionLog::addAction(ActionBase *act)
{
    if(act) {
        if(!isLastAction()) {
            eraseAfterCurrent();
        }

        if(isLastPossible()) {
            sendStatus(tr("The actions log is full. "
                          "Save the changes to resume the log"));
        }
        else if(isFull()) {
            sendStatus(tr("The action isn't logged"));
            return false;
        }

        log.append(act);
        ++pos;

        connect(act, &ActionBase::addWord, this, &ActionLog::addWord);
        connect(act, &ActionBase::deleteWord, this, &ActionLog::deleteWord);

        return true;
    }

    return false;
}

void ActionLog::erase(int begin, int end)
{
    if(begin < 0 || begin >= log.size() || begin > end || end >= log.size()) {
        return;
    }

    for(int i = end; i >= begin; --i) {
        delete log[i];
        log[i] = nullptr;
        log.removeAt(i);
    }

    if(pos >= begin) {
        pos = begin == -1 ? -1 : begin -1;
    }
}

void ActionLog::clear()
{
    if(log.size() == 0) {
        return;
    }

    erase(0, log.size()-1);
    pos = -1;
}

void ActionLog::undo()
{
    if(pos < 0) {
        sendStatus(tr("Already at oldest change"));
        return;
    }

    log[pos]->undo();
    --pos;
}

void ActionLog::redo()
{
    if(isLastAction()) {
        sendStatus(tr("Already at newest change"));
        return;
    }

    ++pos;
    log[pos]->redo();
}

void ActionLog::eraseAfterCurrent()
{
    erase(pos+1, log.size()-1);
}

bool ActionLog::isFull() const
{
    return pos == MaxSize - 1;
}

bool ActionLog::isLastPossible() const
{
    return pos == MaxSize - 2;
}

bool ActionLog::isLastAction() const
{
    return pos == log.size() - 1;
}

ActionLog::ActionLog()
    : pos(-1), log(QList<ActionBase*>())
{

}

ActionLog::~ActionLog()
{
    clear();
}
