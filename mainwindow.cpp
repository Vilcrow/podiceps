/*
podiceps2 - pocket dictionary

Copyright (C) 2022 S.V.I 'Vilcrow', <vilcrow.net>
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

#include <QMenuBar>
#include <QFileDialog>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	dictWidget = new DictionaryWidget;
	setCentralWidget(dictWidget);
	createMenus();
	setWindowTitle("podiceps2");
/*
	QPalette palette = QPalette();
	palette.setColor(QPalette::Window, Qt::gray);
	setAutoFillBackground(true);
	setPalette(palette);
*/
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	openAct = new QAction(tr("&Open..."), this);
	fileMenu->addAction(openAct);
	connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
	saveAct = new QAction(tr("&Save As..."), this);
	fileMenu->addAction(saveAct);
	connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
	fileMenu->addSeparator();
	exitAct = new QAction(tr("E&xit"), this);
	fileMenu->addAction(exitAct);
	connect(exitAct, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if(!fileName.isEmpty())
		dictWidget->readFromFile(fileName);
}

void MainWindow::saveFile()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if(!fileName.isEmpty())
		dictWidget->writeToFile(fileName);
}

void MainWindow::updateActions(const QItemSelection &selection)
{
	QModelIndexList indexes = selection.indexes();
	if(!indexes.isEmpty()) {

	}
	else {
		
	}
}
