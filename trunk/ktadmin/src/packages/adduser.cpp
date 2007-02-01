/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
 
#include "adduser.h"

namespace Packages {

AddUser::AddUser(const QString& login, const QString& password, const QString&name)
{
	QDomElement root = createElement( "adduser" );
	root.setAttribute( "version",  "0" );
	m_info = createElement ("info");
	root.appendChild(m_info);
	m_info.setAttribute("login", login);
	m_info.setAttribute("password", password);
	m_info.setAttribute("name", name);
	appendChild(root);
}


AddUser::~AddUser()
{
}

void AddUser::setLogin(const QString& login)
{
	m_info.setAttribute("login", login);
}

void AddUser::setPassword(const QString& password)
{
	m_info.setAttribute("password", password);
}

void AddUser::setName(const QString& name)
{
	m_info.setAttribute("name", name);
}

}
