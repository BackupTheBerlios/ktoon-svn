/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "ktnewprojectpackage.h"

KTNewProjectPackage::KTNewProjectPackage(const QString & name, const QString & author )
 : QDomDocument()
{
	QDomElement root = createElement("newproject");
	root.setAttribute("version", "0");
	
	m_name = createTextNode(name);
	m_author = createTextNode(author);
	
	root.appendChild(createElement("name")).appendChild(m_name);
	root.appendChild(createElement("author")).appendChild(m_author);
	
	appendChild(root);
}


KTNewProjectPackage::~KTNewProjectPackage()
{
}


void KTNewProjectPackage::setName(const QString & name)
{
	m_name.setData(name);
}

void KTNewProjectPackage::setAuthor(const QString & author)
{
	
	m_author.setData(author);
}