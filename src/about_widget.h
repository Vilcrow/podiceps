/**************************************************************************/
/*  about_widget.h                                                        */
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

#ifndef ABOUT_WIDGET_VIL_H
#define ABOUT_WIDGET_VIL_H

#include <QDialog>

class AboutWidget : public QDialog {
    Q_OBJECT
public:
    AboutWidget(QWidget *parent = nullptr);
    virtual ~AboutWidget();
private slots:
    void openAboutQt();
    void openLicense();
    void openFullText();
private:
    static QString getFullLicenseText();
    static QString getShortLicenseText();
};

#endif
