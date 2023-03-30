/**************************************************************************/
/*  statistics_widget.cpp                                                 */
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

#include "statistics_widget.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

int StatisticsWidget::exec()
{
    updateLabels();

    return QDialog::exec();
}

void StatisticsWidget::setTotal(int n)
{
    totalCount = n;
}

void StatisticsWidget::setNew(int n)
{
    newCount = n;
}

void StatisticsWidget::setMiddle(int n)
{
    middleCount = n;
}

void StatisticsWidget::setLearned(int n)
{
    learnedCount = n;
}

void StatisticsWidget::setAll(int pTotal, int pNew, int pMiddle, int pLearned)
{
    totalCount = pTotal;
    newCount = pNew;
    middleCount = pMiddle;
    learnedCount = pLearned;
}

void StatisticsWidget::updateLabels()
{
    if(totalCount == 0) {
        newNum->setNum(0);
        middleNum->setNum(0);
        learnedNum->setNum(0);
        totalNum->setNum(0);

        return;
    }

    double newPercent = (double)newCount / (double)totalCount * 100;
    double middlePercent = (double)middleCount / (double)totalCount * 100;
    double learnedPercent = (double)learnedCount / (double)totalCount * 100;

    QString newText = QString("%1 (%2%)").arg(newCount)
                                         .arg(newPercent, 0, 'f', 2);
    QString middleText = QString("%1 (%2%)").arg(middleCount)
                                            .arg(middlePercent, 0, 'f', 2);
    QString learnedText = QString("%1 (%2%)").arg(learnedCount)
                                             .arg(learnedPercent, 0, 'f', 2);
    QString totalText = QString("%1").arg(totalCount);

    newNum->setText(newText);
    middleNum->setText(middleText);
    learnedNum->setText(learnedText);
    totalNum->setText(totalText);
}

StatisticsWidget::StatisticsWidget(QWidget *parent)
    : QDialog(parent), totalCount(0), newCount(0),
    middleCount(0), learnedCount(0), newNum(new QLabel),
    middleNum(new QLabel), learnedNum(new QLabel),
    totalNum(new QLabel)
{
    setWindowTitle(tr("Statistics"));

    QGridLayout *mainLayout = new QGridLayout;

    QLabel *newLabel = new QLabel(tr("New"));
    QLabel *middleLabel = new QLabel(tr("Middle"));
    QLabel *learnedLabel = new QLabel(tr("Learned"));
    QLabel *totalLabel = new QLabel(tr("Total"));

    mainLayout->addWidget(newLabel, 0, 0);
    mainLayout->addWidget(middleLabel, 1, 0);
    mainLayout->addWidget(learnedLabel, 2, 0);
    mainLayout->addWidget(totalLabel, 3, 0);

    mainLayout->addWidget(newNum, 0, 1);
    mainLayout->addWidget(middleNum, 1, 1);
    mainLayout->addWidget(learnedNum, 2, 1);
    mainLayout->addWidget(totalNum, 3, 1);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QAbstractButton::clicked, this, &QDialog::accept);

    mainLayout->addWidget(closeButton, 4, 1);

    setLayout(mainLayout);
}

StatisticsWidget::~StatisticsWidget()
{

}
