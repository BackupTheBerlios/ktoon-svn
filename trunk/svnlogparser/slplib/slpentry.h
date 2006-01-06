/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SLPENTRY_H
#define SLPENTRY_H

#include <QDate>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class SLPEntry
{
	public:
		SLPEntry();
		~SLPEntry();
		void setRevision(int revision);
		void setAuthor(const QString &author);
		void setMsg(const QString &msg);
		void setDate(const QDateTime &date);
		
		int revision() const;
		QString author() const;
		QString msg() const;
		QDateTime date() const;
		
		void debug();
		
	private:
		int m_revision;
		QString m_author, m_msg;
		QDateTime m_date;
};

typedef QList<SLPEntry> SLPEntries;

#endif
