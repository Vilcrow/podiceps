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
#include "about_widget.h"
#include "dictionary_widget.h"
#include "preferences_widget.h"
#include "save_dialog.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QResizeEvent>
#include <QStatusBar>
#include <QVBoxLayout>

void MainWindow::appendFormat(QString &name, const QString &format)
{
    if(format.isEmpty()) {
        return;
    }

    if(!name.endsWith(format, Qt::CaseInsensitive)) {
        name.append("." + format);
    }
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

        if(dictWidget->hasSelectedWords()) {
            deleteAct->setEnabled(true);
        }
        else {
            deleteAct->setEnabled(false);
        }
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

        editAct->setEnabled(false);
        deleteAct->setEnabled(false);
    }
}

void MainWindow::openFile()
{
    bool ok = true;
    if(!dictWidget->isSaved()) {
        ok = trySaveChanges();
    }

    if(ok) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "", tr("XML Files (*.xml)"));
        if(!fileName.isEmpty()) {
            appendFormat(fileName, "xml");
            dictWidget->readFromFile(fileName);
        }
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

bool MainWindow::saveChanges()
{
    bool success = false;

    QString fileName = dictWidget->getLastFileName();
    if(fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                         "", tr("XML Files (*.xml)"));
    }

    if(!fileName.isEmpty()) {
        appendFormat(fileName, "xml");
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

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                             "", tr("XML Files (*.xml)"));
    if(!fileName.isEmpty()) {
        appendFormat(fileName, "xml");
        dictWidget->writeToFile(fileName);
    }
}

bool MainWindow::trySaveChanges()
{
    bool success = false;

    SaveDialog saveDialog(this);
    QString fileName = dictWidget->getLastFileName();
    int result = saveDialog.askSave();
    switch(result) {
    case SaveDialog::Save:
        success = saveChanges();
        break;
    case SaveDialog::Ignore:
        success = true;
        break;
    case SaveDialog::Cancel:
        success = false;
        break;
    }

    return success;
}

void MainWindow::showMessage(const QString &msg, int timeout)
{
    statusBar->showMessage(msg, timeout);
}

void MainWindow::showStatistics()
{
    int count = dictWidget->getRowCount();
    showMessage(tr("Word count: %1").arg(count));
}

void MainWindow::openTutorial()
{

}

void MainWindow::openAbout()
{
    AboutWidget aboutWidget(this);
    aboutWidget.exec();
}

void MainWindow::openPreferences()
{
    PreferencesWidget preferences(this);
    if(preferences.exec() == QDialog::Accepted) {
        emit preferencesChanged();
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
        writeSettings();
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
        writeSettings();
    }
    if(!ok) {
        event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit resized(width(), height());

    QMainWindow::resizeEvent(event);
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
    saveAsAct->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    fileMenu->addAction(saveAsAct);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(Qt::CTRL + Qt::Key_W);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &MainWindow::quitApp);
}

void MainWindow::createEditMenu()
{
    editMenu = menuBar()->addMenu(tr("&Edit"));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Z);
    editMenu->addAction(undoAct);
    connect(undoAct, &QAction::triggered,
            dictWidget, &DictionaryWidget::undoRequested);

    redoAct = new QAction(tr("Redo"), this);
    redoAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Y);
    editMenu->addAction(redoAct);
    connect(redoAct, &QAction::triggered,
            dictWidget, &DictionaryWidget::redoRequested);

    editMenu->addSeparator();

    addAct = new QAction(tr("&Add..."), this);
    addAct->setShortcut(Qt::CTRL + Qt::Key_A);
    editMenu->addAction(addAct);
    connect(addAct, SIGNAL(triggered()),
            dictWidget, SIGNAL(addWordRequested()));

    findAct = new QAction(tr("&Find..."), this);
    findAct->setShortcut(Qt::CTRL + Qt::Key_F);
    editMenu->addAction(findAct);
    connect(findAct, &QAction::triggered,
            dictWidget, &DictionaryWidget::openFindWidget);

    editMenu->addSeparator();

    editAct = new QAction(tr("&Edit"), this);
    editAct->setShortcut(Qt::CTRL + Qt::Key_E);
    editMenu->addAction(editAct);
    connect(editAct, &QAction::triggered,
            dictWidget, &DictionaryWidget::editWordRequested);

    deleteAct = new QAction(tr("&Delete"), this);
    deleteAct->setShortcut(Qt::Key_Delete);
    editMenu->addAction(deleteAct);
    connect(deleteAct, &QAction::triggered,
            dictWidget, &DictionaryWidget::deleteWordRequested);

    editMenu->addSeparator();

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
    clearInputAct->setShortcut(Qt::Key_Escape);
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

void MainWindow::readSettings()
{

}

void MainWindow::writeSettings()
{
    dictWidget->writeSettings();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mainWindowSettings("Vilcrow", "podiceps"),
      closeImmediately(false)
{
    readSettings();

    setWindowTitle("podiceps");

    dictWidget = new DictionaryWidget(this);
    setCentralWidget(dictWidget);

    createMenus();

    statusBar = new QStatusBar;
    setStatusBar(statusBar);

    updateActions();

    connect(dictWidget, SIGNAL(actionCompleted(const QString&)),
            this, SLOT(showMessage(const QString&)));
    connect(dictWidget, &DictionaryWidget::stateChanged,
            this, &MainWindow::updateActions);
    connect(this, &MainWindow::preferencesChanged,
            dictWidget, &DictionaryWidget::updateSettings);
    connect(this, &MainWindow::resized,
            dictWidget, &DictionaryWidget::resize);
}

MainWindow::~MainWindow()
{

}
