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

struct KTNewProjectPackage::Private
{
	QDomText name, author, description;
};

KTNewProjectPackage::KTNewProjectPackage(const QString & name, const QString & author, const QString & description ): QDomDocument(), d(new Private)
{
	QDomElement root = createElement("newproject");
	root.setAttribute("version", "0");
	
	d->name = createTextNode(name);
	d->author = createTextNode(author);
	d->description = createTextNode(description);
	
	root.appendChild(createElement("name")).appendChild(d->name);
	root.appendChild(createElement("author")).appendChild(d->author);
	root.appendChild(createElement("description")).appendChild(d->description);
	appendChild(root);
}


KTNewProjectPackage::~KTNewProjectPackage()
{
}


void KTNewProjectPackage::setName(const QString & name)
{
	d->name.setData(name);
}

void KTNewProjectPackage::setAuthor(const QString & author)
{
	d->author.setData(author);
}

void KTNewProjectPackage::setDescription(const QString & description)
{
	d->description.setData(description);
}

