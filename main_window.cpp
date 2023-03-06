/**************************************************************************/
/*  main_window.cpp                                                       */
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

#include "main_window.h"
#include "dictionary_widget.h"
#include "preferences_widget.h"
#include "save_dialog.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>

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
    connect(newAct, &QAction::triggered, this, &MainWindow::createFile);
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
    connect(exitAct, &QAction::triggered, this, &MainWindow::quitApp);
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
    showStatisticsAct = new QAction(tr("&Statistics"), this);
    showStatisticsAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
    toolsMenu->addAction(showStatisticsAct);
    connect(showStatisticsAct, &QAction::triggered,
            this, &MainWindow::showStatistics);
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
    openTutorialAct->setEnabled(false);
    connect(openTutorialAct, &QAction::triggered,
            this, &MainWindow::openTutorial);
    openAboutAct = new QAction(tr("&About"), this);
    helpMenu->addAction(openAboutAct);
    connect(openAboutAct, &QAction::triggered, this, &MainWindow::openAbout);
}

void MainWindow::openFile()
{
    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }

    if(ok) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()) {
            dictWidget->readFromFile(fileName);
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(!fileName.isEmpty()) {
        dictWidget->writeToFile(fileName);
    }
}

void MainWindow::showMessage(const QString &msg)
{
    statusBar->showMessage(msg);
}

void MainWindow::showStatistics()
{
    int count = dictWidget->getRowCount();
    statusBar->showMessage(tr("Word count: %1").arg(count));
}

void MainWindow::openTutorial()
{

}

void MainWindow::openAbout()
{
    QDialog *aboutWidget = new QDialog(this);
    aboutWidget->setFixedSize(500, 150);
    aboutWidget->setWindowTitle(tr("About"));
    QString copyrightChar = QChar(0x00A9);
    QLabel aboutLabel("podiceps\n"
                      + tr("Copyright") + copyrightChar
                      + " 2022-2023 Vilcrow\n"
                      + tr("A simple program for maintaining"
                      " a dictionary of foreign words.\n")
                      + tr("License: GPL-3.0-or-later"));
    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(100);
    connect(closeButton, &QAbstractButton::clicked,
            aboutWidget, &QDialog::accept);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(&aboutLabel);
    vLayout->addWidget(closeButton, Qt::AlignCenter);
    aboutWidget->setLayout(vLayout);
    aboutWidget->exec();
}

void MainWindow::importFile()
{
    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }

    if(ok) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()) {
            dictWidget->importFromFile(fileName);
            dictWidget->setLastFileName("");
        }
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

void MainWindow::openPreferences()
{
    PreferencesWidget preferences(this);
    if(preferences.exec() == QDialog::Accepted) {
        emit preferencesChanged();
    }
}

bool MainWindow::saveChanges()
{
    bool success = false;

    QString fileName = dictWidget->getLastFileName();
    if(fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this);
    }

    if(!fileName.isEmpty()) {
        dictWidget->writeToFile(fileName);
        dictWidget->setSaved(true);
        updateActions();
        success = true;
    }
    else {
        success = false;
    }

    return success;
}

bool MainWindow::trySaveChanges()
{
    bool success = false;

    SaveDialog sDialog(this);
    QString fileName = dictWidget->getLastFileName();
    int result = sDialog.trySave();
    switch(result) {
    case SaveDialog::Accepted:
        result = saveChanges();
        break;
    case SaveDialog::Rejected:
        success = true;
        break;
    case SaveDialog::Cancelled:
        success = false;
        break;
    }

    return success;
}

void MainWindow::updateActions()
{
    if(dictWidget->getRowCount() != 0) {
        newAct->setEnabled(true);
        if(!dictWidget->isSaved() && !dictWidget->getLastFileName().isEmpty()) {
            saveAct->setEnabled(true);
        }
        else {
            saveAct->setEnabled(false);
        }
        saveAsAct->setEnabled(true);
        exportAct->setEnabled(true);
    }
    else {
        newAct->setEnabled(false);
        if(!dictWidget->isSaved() && !dictWidget->getLastFileName().isEmpty()) {
            saveAct->setEnabled(true);
        }
        else {
            saveAct->setEnabled(false);
        }
        saveAsAct->setEnabled(false);
        exportAct->setEnabled(false);
    }
}

void MainWindow::createFile()
{
    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }

    if(ok) {
        dictWidget->createNewFile();
    }
}

void MainWindow::quitApp()
{
    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }
    if(ok) {
        closeImmediately = true;
        close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(closeImmediately) {
        return;
    }

    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }
    if(!ok) {
        event->ignore();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mainWindowSettings("Vilcrow", "podiceps")
{
    closeImmediately = false;
    readSettings();
    setWindowTitle("podiceps");
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
    connect(this, &MainWindow::preferencesChanged,
            dictWidget, &DictionaryWidget::updateSettings);
}

MainWindow::~MainWindow()
{
    writeSettings();
}
