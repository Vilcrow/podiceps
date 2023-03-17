/**************************************************************************/
/*  status_spin_box.h                                                     */
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

#ifndef STATUS_SPIN_BOX_VIL_H
#define STATUS_SPIN_BOX_VIL_H

#include <QSpinBox>

class StatusSpinBox : public QSpinBox {
    Q_OBJECT
public:
    QString getValue() const;
    void setValue(const QString &value);

    StatusSpinBox(QWidget *parent = nullptr);
    virtual ~StatusSpinBox();
signals:
    void returnPressed();
private slots:
    void setColor(int status);
protected:
    QString textFromValue(int value) const override;
    int valueFromText(const QString &text) const override;
    void keyPressEvent(QKeyEvent *event);
};

#endif
