#ifndef MAINWINDOW_H_SENTRY
#define MAINWINDOW_H_SENTRY

#include "dictionarywidget.h"
#include <QMainWindow>
#include <QItemSelection>

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
private slots:
	void updateActions(const QItemSelection &selection);
	void openFile();
	void saveFile();
private:
	void createMenus();

	DictionaryWidget *dictWidget;
	QMenu *fileMenu;
	QAction *openAct;
	QAction *saveAct;
	QAction *exitAct;
};

#endif
