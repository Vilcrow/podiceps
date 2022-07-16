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

MainWindow::MainWindow() : mainWindowSettings("Vilcrow", "podiceps2")
{
	readSettings();
	setWindowTitle("podiceps2");
	dictWidget = new DictionaryWidget;
	setCentralWidget(dictWidget);
	createMenus();
	statusBar = new QStatusBar;
	setStatusBar(statusBar);
	//setMinimumHeight(500);
	//setMinimumWidth(500); don't work. Why?
	connect(dictWidget, &DictionaryWidget::sendMessage, this, &MainWindow::showMessage);
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	newAct = new QAction(tr("&New..."), this);
	newAct->setShortcut(Qt::CTRL + Qt::Key_N);
	fileMenu->addAction(newAct);
	connect(newAct, &QAction::triggered, dictWidget, &DictionaryWidget::createNewFile);
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(Qt::CTRL + Qt::Key_O);
	fileMenu->addAction(openAct);
	connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
	saveAsAct = new QAction(tr("&Save As..."), this);
	saveAsAct->setShortcut(Qt::CTRL + Qt::Key_S);
	fileMenu->addAction(saveAsAct);
	connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFile);
	fileMenu->addSeparator();
	importAct = new QAction(tr("&Import..."), this);
	importAct->setShortcut(Qt::CTRL + Qt::Key_I);
	fileMenu->addAction(importAct);
	connect(importAct, &QAction::triggered, this, &MainWindow::importFile);
	exportAct = new QAction(tr("&Export..."), this);
	exportAct->setShortcut(Qt::CTRL + Qt::Key_E);
	fileMenu->addAction(exportAct);
	connect(exportAct, &QAction::triggered, this, &MainWindow::exportFile);
	fileMenu->addSeparator();
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(Qt::CTRL + Qt::Key_W);
	fileMenu->addAction(exitAct);
	connect(exitAct, &QAction::triggered, this, &QWidget::close);
	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	openStatisticsAct = new QAction(tr("&Statistics"), this);
	toolsMenu->addAction(openStatisticsAct);
	connect(openStatisticsAct, &QAction::triggered, this,
			&MainWindow::openStatistics);
	clearInputAct = new QAction(tr("&Clear input"), this);
	clearInputAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
	toolsMenu->addAction(clearInputAct);
	connect(clearInputAct, &QAction::triggered, dictWidget, &DictionaryWidget::clearInput);
	helpMenu = menuBar()->addMenu(tr("&Help"));
	openTutorialAct = new QAction(tr("&Tutorial"), this);
	helpMenu->addAction(openTutorialAct);
	connect(openTutorialAct, &QAction::triggered, this,
			&MainWindow::openTutorial);
	openAboutAct = new QAction(tr("&About"), this);
	helpMenu->addAction(openAboutAct);
	connect(openAboutAct, &QAction::triggered, this,
			&MainWindow::openAbout);
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

void MainWindow::showMessage(const QString &msg)
{
	statusBar->showMessage(msg);
}

void MainWindow::openStatistics()
{

}

void MainWindow::openTutorial()
{

}

void MainWindow::openAbout()
{

}

void MainWindow::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if(!fileName.isEmpty())
		dictWidget->importFromFile(fileName);
}

void MainWindow::exportFile()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if(!fileName.isEmpty())
		dictWidget->exportToFile(fileName);
}

void MainWindow::readSettings()
{
/*
	mainSettings.beginGroup("/Settings");
	mainSettings.endGroup();
*/
}

void MainWindow::writeSettings()
{
/*
	mainSettings.beginGroup("/Settings");
	mainSettings.endGroup();
*/
}

MainWindow::~MainWindow()
{
	writeSettings();
}
