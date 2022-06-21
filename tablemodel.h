#ifndef TABELMODEL_H_SENTRY
#define TABLEMODEL_H_SENTRY

#include <QAbstractTableModel>
#include <QList>
#include <QString>

class Word {
	QString original;
	QString translation;
	QString status;
	QString date;
public:
	Word(const QString originalArg, const QString translationArg,
		 const QString statusArg = tr("new"));
	inline QString& getOriginal() const { return original; }
	inline QString& getTranslation() const { return translation; }
	inline QString& getStatus() const { return status; }
	inline QString& getDate() const { return date; }
	bool operator==(const Word &other) const;
};

inline QDataStream &operator<<(QDataStream &stream, const Word &word)
{
	return stream << word.getOriginal() << word.getTranslation()
				  << word.getStatus() << word.getDate();
}

inline QDataStream &operator>>(QDataStream &stream, Word &word)
{
	return stream >> word.getOriginal() >> word.getTranslation()
				  >> word.getStatus() >> word.getDate();
}

class TableModel : public QAbstractTabelModel {
	Q_OBJECT
public:
	TableModel(QObject *parent = nullptr);
	TableModel(QList<Word> words, QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	bool insertRows(int position, int rows,
					const QModelIndex &index = QModelIndex()) override;
	bool removeRows(int position, int rows,
					const QModelIndex &index = QModelIndex()) override;
	QList<Word> getWords() const;
private:
	QList<Word> words;
};

#endif
