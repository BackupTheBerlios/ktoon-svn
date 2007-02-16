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
#ifndef REGISTERSDATABASE_H
#define REGISTERSDATABASE_H

#include <QString>
#include <QHash>
#include <ktxmlparserbase.h>

namespace Registers {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Database : public KTXmlParserBase
{
	public:
		Database(const QString &dbfile);
		~Database();
		
		QString fileName() const;
		void removeRegister(const QString &email);
		
		bool startTag(const QString &tag, const QXmlAttributes &atts);
		bool endTag(const QString &tag);
		void text(const QString &msg);
		
		QHash<QString, QString> findRegisterByEmail(const QString &email);
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
