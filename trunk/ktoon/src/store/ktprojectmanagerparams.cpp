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

#include "ktprojectmanagerparams.h"

struct KTProjectManagerParams::Private
{
	QString projectName;
	QString author;
	QString description;
};



KTProjectManagerParams::KTProjectManagerParams(): d(new Private)
{
}


KTProjectManagerParams::~KTProjectManagerParams()
{
}


void KTProjectManagerParams::setProjectName(const QString &name)
{
	d->projectName = name;
}

QString KTProjectManagerParams::projectName() const
{
	return d->projectName;
}


void KTProjectManagerParams::setAuthor(const QString &author)
{
	d->author = author;
}

QString KTProjectManagerParams::author() const
{
	return d->author;
}


void KTProjectManagerParams::setDescription(const QString &description)
{
	d->description = description;
}

QString KTProjectManagerParams::description() const
{
	return d->description;
}
