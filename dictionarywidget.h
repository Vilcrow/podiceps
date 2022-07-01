#ifndef DICTIONARYWIDGET_H_SENTRY
#define DICTIONARYWIDGET_H_SENTRY

#include "tablemodel.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QHeaderView>

class DictionaryWidget : public QWidget {
	Q_OBJECT
public:
	DictionaryWidget();
	void readFromFile(const QString &fileName);
	void addEntry(QString original, QString translation,
					QString status, QString date);
private slots:
	void addEntrySlot();
	void editEntry();
private:
	TableModel *table;
	QLabel *originalLabel;
	QLabel *translationLabel;
	QLabel *statusLabel;
	QLabel *dateLabel;
	QLineEdit *originalLineEdit;
	QLineEdit *translationLineEdit;
	QLineEdit *statusLineEdit;
	QLineEdit *dateLineEdit;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *deleteButton;
	QVBoxLayout *mainLayout;
	QGridLayout *gridLayout;
	QHBoxLayout *buttonsLayout;
	QSortFilterProxyModel *proxyModel;
	QTableView *tableView;

	void createLineEditWidgets();
	void createButtons();
	void setupTable();
};

#endif
