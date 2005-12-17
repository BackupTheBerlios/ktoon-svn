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



#include "ktooninstaller.h"

#include <QImage>
#include <QMenu>
#include <qmenubar.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qtextstream.h>
#include <qpainter.h>

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"


KToonInstaller::KToonInstaller() : KTWizard(0)
{
	m_welcomePage = new WelcomePage(this);
	addPage( m_welcomePage );
	
	m_licencePage = new LicencePage(this);
	addPage(m_licencePage);
	
	m_installPage = new InstallPage(this);
	addPage(m_installPage);
}


KToonInstaller::~KToonInstaller()
{
}


void KToonInstaller::closeEvent( QCloseEvent *e )
{
}

