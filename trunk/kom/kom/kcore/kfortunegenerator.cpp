/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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
#include "kfortunegenerator.h"

#include <QProcess>

KFortuneGenerator *KFortuneGenerator::s_self = 0;

struct KFortuneGenerator::Private
{
	bool isValid;
	QString fortunePath;
};


KFortuneGenerator::KFortuneGenerator() : k(new Private())
{
	findFortunePath();
}


KFortuneGenerator::~KFortuneGenerator()
{
	delete k;
}

KFortuneGenerator *KFortuneGenerator::self()
{
	if ( !s_self )
		s_self = new KFortuneGenerator;
	
	return s_self;
}

void KFortuneGenerator::findFortunePath()
{
	k->fortunePath = "fortune";
	if ( QProcess::execute(k->fortunePath) == 0 )
	{
		k->isValid = true;
	}
	else
	{
		k->isValid = false;
	}
}

QString KFortuneGenerator::generate()
{
	if ( !k->isValid )
		return "";
	
	QProcess proc;
	proc.start(k->fortunePath);
	
	proc.waitForFinished();
	
	return proc.readAllStandardOutput();
}
