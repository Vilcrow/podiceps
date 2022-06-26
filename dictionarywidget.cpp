#include <QMessageBox>
#include "dictionarywidget.h"
#include "mainwindow.h"

DictionaryWidget::DictionaryWidget()
{
	table = new TableModel(this);
	setupTable();
	mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(tableView);
	createLineEditWidgets();
	mainLayout->addLayout(gridLayout);
	createButtons();
	mainLayout->addLayout(buttonsLayout);
}

void DictionaryWidget::createLineEditWidgets()
{
	originalLabel = new QLabel(tr("Original"));
	translationLabel = new QLabel(tr("Translation"));
	statusLabel = new QLabel(tr("Status"));
	dateLabel = new QLabel(tr("Date"));
	originalLineEdit = new QLineEdit();
	translationLineEdit = new QLineEdit();
	statusLineEdit = new QLineEdit();
	dateLineEdit = new QLineEdit();
	gridLayout = new QGridLayout();
	gridLayout->addWidget(originalLabel, 0, 0, Qt::AlignLeft);
	gridLayout->addWidget(translationLabel, 0, 1, Qt::AlignLeft);
	gridLayout->addWidget(statusLabel, 0, 2, Qt::AlignLeft);
	gridLayout->addWidget(dateLabel, 0, 3, Qt::AlignLeft);
	gridLayout->addWidget(originalLineEdit, 1, 0, Qt::AlignLeft);
	gridLayout->addWidget(translationLineEdit, 1, 1, Qt::AlignLeft);
	gridLayout->addWidget(statusLineEdit, 1, 2, Qt::AlignLeft);
	gridLayout->addWidget(dateLineEdit, 1, 3, Qt::AlignLeft);
}

void DictionaryWidget::createButtons()
{
	buttonsLayout = new QHBoxLayout();
	addButton = new QPushButton(tr("Add"));
	buttonsLayout->addWidget(addButton);
	editButton = new QPushButton(tr("Edit"));
	buttonsLayout->addWidget(editButton);
}

void DictionaryWidget::setupTable()
{
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(table);
	proxyModel->setFilterRegExp(QRegExp("*", Qt::CaseInsensitive));	//?????
	proxyModel->setFilterKeyColumn(0);
	tableView = new QTableView;
	tableView->setModel(proxyModel);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->verticalHeader()->hide();
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->setSortingEnabled(true);
}

void DictionaryWidget::readFromFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		return;
	}
	QList<Word> words;
	QDataStream in(&file);
	in >> words;
	if(words.isEmpty()) {
		QMessageBox::information(this, tr("No words in file"),
			tr("The file you are attempting to open contains no words."));
	}
	else {
		for(const auto &word: qAsConst(words))
			addEntry(word.original, word.translation, word.status, word.date);
	}
}

void DictionaryWidget::addEntry(QString originalArg, QString translationArg,
								QString statusArg, QString dateArg)
{

}
