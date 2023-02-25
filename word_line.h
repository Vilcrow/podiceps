/**************************************************************************/
/*  word_line.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                               podiceps2                                */
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

#ifndef WORD_LINE_VIL_H
#define WORD_LINE_VIL_H

#include <QString>
#include <QObject>

class WordLine {
public:
	QString getOriginal() const { return original; }
	QString getTranslation() const { return translation; }
	QString getStatus() const { return status; }
	QString getDate() const { return date; }

	void setOriginal(const QString &pOriginal) { original = pOriginal; }
	void setTranslation(const QString &pTranslation) {
		translation = pTranslation;
	}
	void setStatus(const QString &pStatus) { status = pStatus; }
	void setDate(const QString &pDate) { date = pDate; }

	bool operator==(const WordLine &other) const;
	bool operator>(const WordLine &other) const;
	bool operator<(const WordLine &other) const;
	bool operator>=(const WordLine &other) const;
	bool operator<=(const WordLine &other) const;

	QDataStream& operator<<(QDataStream &stream) const;
	QDataStream& operator>>(QDataStream &stream);

	WordLine();
	WordLine(const QString pOriginal,
			 const QString pTranslation = QString(),
			 const QString pStatus = QString(QObject::tr("new")),
			 const QString pDate = QString()
			);
private:
	QString original;
	QString translation;
	QString status;
	QString date;
};

inline QDataStream &operator<<(QDataStream &stream, const WordLine &word)
{
	return stream << word;
}

inline QDataStream &operator>>(QDataStream &stream, WordLine &word)
{
	return stream >> word;
}

#endif
