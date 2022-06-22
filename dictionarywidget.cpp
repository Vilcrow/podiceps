#include "dictionarywidget.h"

DictionaryWidget::DictionaryWidget()
{
	tableModel = new TableModel();
	mainLayout = new QVBoxLayout();
	mainLayout->addWidget(tableModel);
	createLineEditWidgets();
	mainLayout->setLayout(gridLayout);
	createButtons();
	mainLayout->setLayout(buttonsLayout);
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
	buttonLayout->addWidget(addButton);
	editButton = new QPushButton(tr("Edit"));
	buttonLayout->addWidget(editButton);
	quitButton = new QPushButton(tr("Quit"));
	buttonLayout->addWidget(quitButton);
}
