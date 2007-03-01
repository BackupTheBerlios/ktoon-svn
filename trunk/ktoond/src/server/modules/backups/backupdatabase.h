/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef SERVERBACKUPDATABASE_H
#define SERVERBACKUPDATABASE_H

#include <ktxmlparserbase.h>

#include <QDateTime>
#include <QHash>

namespace Backups {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Database : public KTXmlParserBase
{
	public:
		struct Entry
		{
			QString file;
			QDateTime date;
			QString origin;
		};
		
		Database(const QString &file);
		~Database();
		
		bool startTag(const QString &tag, const QXmlAttributes &atts);
		bool endTag(const QString &tag);
		void text(const QString &msg);
		
		bool addEntry(const QString &origFile, const QString &filename, const QString &name, const QDateTime &date);
		bool removeEntry(const QString &name, const QDateTime &date);
		
		QHash<QString, QList<Entry> > entries();
		
	private:
		struct Private;
		Private *const d;
};

}

#endif