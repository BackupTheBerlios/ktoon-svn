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

#ifndef USERMANAGER_H_
#define USERMANAGER_H_

#include <QString>

namespace Users {
class User;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Manager
{
	public:
		Manager(const QString &dbfile);
		~Manager();
		
		bool auth(const QString &login, const QString &password);
		User *user(const QString &login);
		User *loadUser(const QString &login);
		
		QList<User*> users() const;
		
		void addUser( const User & user );
		void updateUser( const User & user );
		void removeUser(const QString &login);
		
		QList<User*> listUsers();
		
		
	private:
		struct Private;
		Private *const d;
};

}

#endif


