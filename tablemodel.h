#ifndef TABLEMODEL_H_SENTRY
#define TABLEMODEL_H_SENTRY

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QList>
#include <QString>

struct Word {
	QString original;
	QString translation;
	QString status = QObject::tr("new");
	QString date;
	bool operator==(const Word &other) const
	{
		return original    == other.original &&
			   translation == other.translation &&
			   status      == other.status &&
			   date        == other.date;
	}
};

inline QDataStream &operator<<(QDataStream &stream, const Word &word)
{
	return stream << word.original << word.translation
				  << word.status << word.date;
}

inline QDataStream &operator>>(QDataStream &stream, Word &word)
{
	return stream >> word.original >> word.translation
				  >> word.status >> word.date;
}

class TableModel : public QAbstractTableModel {
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
