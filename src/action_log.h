/**************************************************************************/
/*  action_log.h                                                          */
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

#ifndef ACTION_LOG_VIL_H
#define ACTION_LOG_VIL_H

#include "action_base.h"
#include <QList>
#include <QObject>
#include <QString>

class ActionLog : public QObject {
    Q_OBJECT
public:
    enum { MaxSize = 1000 };

    bool addAction(ActionBase *act);
    void erase(int begin, int end);
    void clear();

    bool isEmpty() const;

    void undo();
    void redo();

    ActionLog();
    virtual ~ActionLog();
signals:
    void addWord(const WordLine &word, bool makeLog = false);
    void deleteWord(const WordLine &word, bool makeLog = false);
    void sendStatus(const QString &msg);
private:
    int pos;
    QList<ActionBase*> log;

    void eraseAfterCurrent();
    bool isFull() const;
    bool isLastPossible() const;
    bool isLastAction() const;
};

#endif
