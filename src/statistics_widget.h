/**************************************************************************/
/*  statistics_widget.h                                                   */
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

#ifndef STATISTICS_WIDGET_VIL_H
#define STATISTICS_WIDGET_VIL_H

#include <QDialog>

class QLabel;

class StatisticsWidget : public QDialog {
    Q_OBJECT
public:
    void setTotal(int n);
    void setNew(int n);
    void setMiddle(int n);
    void setLearned(int n);
    void setAll(int pTotal, int pNew, int pMiddle, int pLearned);

    StatisticsWidget(QWidget *parent = nullptr);
    virtual ~StatisticsWidget();
public slots:
    int exec() override;
private:
    int totalCount;
    int newCount;
    int middleCount;
    int learnedCount;

    QLabel *newNum;
    QLabel *middleNum;
    QLabel *learnedNum;
    QLabel *totalNum;

    void updateLabels();
};

#endif
