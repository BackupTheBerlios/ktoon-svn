/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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
#include "ktmdiwindow.h"
#include "ktdebug.h"
#include <qmenubar.h>
//Added by qt3to4:
#include <QEvent>

KTMdiWindow::KTMdiWindow(QWidget* parent, const char* name, Qt::WFlags f): QMainWindow(parent, name, f)
{
	
}


KTMdiWindow::~KTMdiWindow()
{
	
}

bool  KTMdiWindow::event( QEvent * e )
{
	if ( e->type() == QEvent::Hide )
	{
		emit activate(false);
	}
	else if ( e->type() == QEvent::Show )
	{
		adjustSize();
		emit activate(true);
	}
// 	else if( e->type() == QEvent::ShowMaximized )
// 	{
// 		if(menuBar())
// 			menuBar()->hide();
// 	}
// 	else if(e->type() == QEvent::ShowNormal )
// 	{
// 		if(menuBar())
// 			menuBar()->show();
// 	}

	return QMainWindow::event(e );
	
	
}
