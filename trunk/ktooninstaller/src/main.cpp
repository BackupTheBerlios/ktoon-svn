/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                *
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

#include "application.h"
#include "ktooninstaller.h"
#include "ktdebug.h"

#if !defined(Q_OS_UNIX)
#error Only supports unix systems
#endif

int main( int argc, char ** argv )
{
	Application app( argc, argv );
	
	if ( app.ktoonHome().isEmpty() )
	{
		ktFatal() << QObject::tr("Please don't use this binary directly");
		return -1;
	}
	
	KToonInstaller * mw = new KToonInstaller();
	mw->setWindowTitle( QObject::tr("KToon Installer = )"));
	mw->show();
	
	return app.exec();
}






