#ifndef DICTIONARYWIDGET_H_SENTRY
#define DICTIONARYWIDGET_H_SENTRY

class DictionaryWidget : public QWidget {
	Q_OBJECT
public:
	DictionaryWidget();
private:
	TableModel *tableModel;
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
	QPushButton *quitButton;
	QVBoxLayout *mainLayout;
	QGridLayout *gridLayout;
	QHBoxLayout *buttonsLayout;
	void createLineEditWidgets();
	void createButtons();
};

#endif
