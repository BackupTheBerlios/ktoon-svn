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


#include "projects.h"
namespace Packages
{

ProjectList::ProjectList() : Package()
{
	QDomElement root = createElement ( "projectlist" );
	root.setAttribute ( "version",  "0" );
	appendChild(root);
}


ProjectList::~ProjectList()
{
}

void ProjectList::addProject(const QString & name, const QString &author, const QString &description )
{
	QDomNode root = firstChild ();
	
	QDomElement projectE = createElement( "project" );
	projectE.setAttribute("name", name);
	projectE.setAttribute("author", author);
	projectE.setAttribute("description", description);
	
	root.appendChild(projectE);
}

}
