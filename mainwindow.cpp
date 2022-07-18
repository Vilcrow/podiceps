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
	updateActions();
	connect(dictWidget, &DictionaryWidget::sendMessage,
			this, &MainWindow::showMessage);
	connect(dictWidget, &DictionaryWidget::updateMenus,
			this, &MainWindow::updateActions);
	if(!dictWidget->getLastFileName().isEmpty())
		showMessage(tr("The file \"%1\" is open")
						.arg(dictWidget->getLastFileName()));
}

void MainWindow::createMenus()
{
	createFileMenu();
	createEditMenu();
	createToolsMenu();
	createHelpMenu();
}

void MainWindow::createFileMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	newAct = new QAction(tr("&New..."), this);
	newAct->setShortcut(Qt::CTRL + Qt::Key_N);
	fileMenu->addAction(newAct);
	connect(newAct, &QAction::triggered,
			dictWidget, &DictionaryWidget::createNewFile);
	connect(newAct, &QAction::triggered, this, &MainWindow::updateActions);
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(Qt::CTRL + Qt::Key_O);
	fileMenu->addAction(openAct);
	connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
	fileMenu->addAction(saveAct);
	connect(saveAct, &QAction::triggered, this, &MainWindow::saveChanges);
	saveAsAct = new QAction(tr("S&ave As..."), this);
	//saveAsAct->setShortcut(Qt::CTRL + Qt::Key_S);
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
}

void MainWindow::createEditMenu()
{
	editMenu = menuBar()->addMenu(tr("&Edit"));
	preferencesAct = new QAction(tr("&Preferences"), this);
	preferencesAct->setShortcut(Qt::CTRL + Qt::Key_P);
	editMenu->addAction(preferencesAct);
	connect(preferencesAct, &QAction::triggered,
			this, &MainWindow::openPreferences);
}

void MainWindow::createToolsMenu()
{
	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	openStatisticsAct = new QAction(tr("&Statistics"), this);
	toolsMenu->addAction(openStatisticsAct);
	connect(openStatisticsAct, &QAction::triggered, this,
			&MainWindow::openStatistics);
	clearInputAct = new QAction(tr("&Clear input"), this);
	clearInputAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
	toolsMenu->addAction(clearInputAct);
	connect(clearInputAct, &QAction::triggered,
			dictWidget, &DictionaryWidget::clearInput);
}

void MainWindow::createHelpMenu()
{
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
	if(!fileName.isEmpty()) {
		dictWidget->writeToFile(fileName);
		dictWidget->setSaved(true);
		dictWidget->setLastFileName(fileName);
	}
}

void MainWindow::showMessage(const QString &msg)
{
	statusBar->showMessage(msg);
}

void MainWindow::openStatistics()
{
	QDialog *statisticsDialog = new QDialog(this);
	statisticsDialog->setFixedSize(500, 500);
	statisticsDialog->setWindowTitle(tr("Statistics"));
	QLabel *wordCount = new QLabel(tr("Word count: %1")
							.arg(dictWidget->getRowCount()));
	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(wordCount);
	statisticsDialog->setLayout(vLayout);
	statisticsDialog->exec();
}

void MainWindow::openTutorial()
{

}

void MainWindow::openAbout()
{
	QDialog *aboutWindow = new QDialog(this);
	aboutWindow->setFixedSize(500, 150);
	aboutWindow->setWindowTitle(tr("About"));
	QString copyrightChar = QChar(0x00A9);
	QLabel *aboutLabel = new QLabel(
							"podiceps2\n"
							+ tr("Copyright") + copyrightChar
							+ " 2022 Vilcrow\n"
							+ tr("A simple program for maintaining"
							" a dictionary of foreign words.\n")
							+ tr("License: GPL-3.0-or-later"));
	QPushButton *closeButton = new QPushButton(tr("Close"));
	closeButton->setFixedWidth(100);
	connect(closeButton, &QAbstractButton::clicked, aboutWindow, &QDialog::accept);
	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(aboutLabel);
	vLayout->addWidget(closeButton, Qt::AlignCenter);
	aboutWindow->setLayout(vLayout);
	aboutWindow->exec();
}

void MainWindow::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if(!fileName.isEmpty()) {
		dictWidget->importFromFile(fileName);
		dictWidget->setLastFileName("");
	}
}

void MainWindow::exportFile()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if(!fileName.isEmpty()) {
		dictWidget->exportToFile(fileName);
		dictWidget->setLastFileName(fileName);
	}
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

void MainWindow::openPreferences()
{

}

void MainWindow::saveChanges()
{
	QString fileName;
	if(dictWidget->getLastFileName().isEmpty())
		QFileDialog::getSaveFileName(this);
	else
		fileName = dictWidget->getLastFileName();
	if(!fileName.isEmpty()) {
		dictWidget->writeToFile(fileName);
		dictWidget->setSaved(true);
	}
	dictWidget->setSaved(true);
	updateActions();
}

void MainWindow::updateActions()
{
	if(dictWidget->getRowCount() != 0) {
		newAct->setEnabled(true);
		if(!dictWidget->isSaved() && !dictWidget->getLastFileName().isEmpty())
			saveAct->setEnabled(true);
		else
			saveAct->setEnabled(false);
		saveAsAct->setEnabled(true);
		exportAct->setEnabled(true);
	}
	else {
		newAct->setEnabled(false);
		if(!dictWidget->isSaved() && !dictWidget->getLastFileName().isEmpty())
			saveAct->setEnabled(true);
		else
			saveAct->setEnabled(false);
		saveAsAct->setEnabled(false);
		exportAct->setEnabled(false);
	}
}
