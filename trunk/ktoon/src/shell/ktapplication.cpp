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

#include "ktapplication.h"
#include "configwizard.h"
#include <dcore/dapplicationproperties.h>

#include <dcore/ddebug.h>

KTApplication::KTApplication(int &argc, char **argv)
 : DApplication(argc, argv)
{
	setApplicationName("ktoon");
	
// 	applyColors(Default);
}


KTApplication::~KTApplication()
{
}

bool KTApplication::firstRun()
{
	ConfigWizard *firstDialog = new ConfigWizard;
	QApplication::setActiveWindow(firstDialog);

	if ( firstDialog->exec() != QDialog::Rejected )
	{
		dAppProp->setHomeDir( firstDialog->home() );
		createCache(firstDialog->cache());
		
		DCONFIG->beginGroup("General");
		
		DCONFIG->setValue( "Home", HOME_DIR);
		DCONFIG->setValue( "Cache", CACHE_DIR);
		
		DCONFIG->sync();
		
		delete firstDialog;
		
		return true;
	}
	
	delete firstDialog;
	
	return false;
}

void KTApplication::createCache(const QString &cacheDir)
{
	QDir cache(cacheDir);
	if ( ! cache.exists() )
	{
		dDebug() << tr("Initializing repository %1").arg(cacheDir);
		if ( ! cache.mkdir(cacheDir) )
		{
			dError() << tr("I cannot create the repository");
		}
	}
	
	dAppProp->setCacheDir(cacheDir);
}

