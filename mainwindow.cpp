#include <QMenuBar>
#include <QFileDialog>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	dictWidget = new DictionaryWidget;
	setCentralWidget(dictWidget);
	createMenus();
	setWindowTitle("podiceps2");
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	openAct = new QAction(tr("&Open..."), this);
	fileMenu->addAction(openAct);
	connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
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

}

void MainWindow::updateActions(const QItemSelection &selection)
{
	QModelIndexList indexes = selection.indexes();
	if(!indexes.isEmpty()) {

	}
	else {
		
	}
}
