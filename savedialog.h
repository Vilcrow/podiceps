/*
podiceps2 - pocket dictionary

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <vilcrow.net>
--------------------------------------------------------------------------------
LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#ifndef SAVEDIALOG_H_SENTRY
#define SAVEDIALOG_H_SENTRY

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SaveDialog : public QDialog
{
	Q_OBJECT
public:
	SaveDialog(QWidget *parent = 0);
private:
	QLabel *infoLabel;
	QPushButton *saveButton;
	QPushButton *noButton;
	//QPushButton *cancelButton;
};

#endif
