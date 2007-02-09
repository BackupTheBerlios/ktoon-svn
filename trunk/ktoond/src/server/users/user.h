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

#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

#include "right.h"

class QDomDocument;
class QDomElement;

namespace Users {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class User
{
	public:
		explicit User();
		~User();
		
		void setName(const QString &name);
		void setLogin(const QString &login);
		void setPassword(const QString &password);
		
		QString name() const;
		QString login() const;
		QString password() const;
		
		bool canReadOn(const QString &module);
		bool canWriteOn(const QString &module);
		
		void addRight(Right *right);
		QList<Right *> rights() const;
		
		QDomElement toXml(QDomDocument &doc, bool password = true) const;
		
		bool operator==(const User& user);
		
	private:
		struct Private;
		Private *const d;
};

}

#endif


