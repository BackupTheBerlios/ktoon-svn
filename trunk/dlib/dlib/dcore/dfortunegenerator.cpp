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
#include "dfortunegenerator.h"

#include <QProcess>

DFortuneGenerator *DFortuneGenerator::s_self = 0;

struct DFortuneGenerator::Private
{
	bool isValid;
	QString fortunePath;
};


DFortuneGenerator::DFortuneGenerator() : d(new Private())
{
	findFortunePath();
}


DFortuneGenerator::~DFortuneGenerator()
{
	delete d;
}

DFortuneGenerator *DFortuneGenerator::self()
{
	if ( !s_self )
		s_self = new DFortuneGenerator;
	
	return s_self;
}

void DFortuneGenerator::findFortunePath()
{
	d->fortunePath = "fortune";
	if ( QProcess::execute(d->fortunePath) == 0 )
	{
		d->isValid = true;
	}
	else
	{
		d->isValid = false;
	}
}

QString DFortuneGenerator::generate()
{
	if ( !d->isValid )
		return "";
	
	QProcess proc;
	proc.start(d->fortunePath);
	
	proc.waitForFinished();
	
	return proc.readAllStandardOutput();
}

