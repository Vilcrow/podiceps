#include "tablemodel.h"

Word::Word(const QString originalArg, const QString translationArg,
		   const QString statusArg)
{
	original = originalArg;
	translation = translationArg;
	status = statusArg;
	date = QDateTime::currentDateTime().toString("dd-MM-yyyy");
}

bool Word::operator==(const Word &other) const
{
	return original    == other.getOriginal() &&
		   translation == other.getTranslation() &&
		   status      == other.getStatus() &&
		   date        == other.getDate();
}
