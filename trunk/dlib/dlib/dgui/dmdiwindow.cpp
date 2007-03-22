/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   				   *
 *   kuadrosx@toonka.com  	   					   *
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

#include "dmdiwindow.h"
#include "ddebug.h"
#include <QMenu>
#include <QEvent>
#include <QMenuBar>

DMdiWindow::DMdiWindow(QWorkspace* parent, const char* name): QMainWindow(parent), m_workspace(parent)
{
	setObjectName(name);
}


DMdiWindow::~DMdiWindow()
{
	
}

bool  DMdiWindow::event( QEvent * e )
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
	else if( e->type() == QEvent::Show )
	{
		if(menuBar())
		{
			if ( isMaximized() )
			{
				menuBar()->hide();
			}
			else
			{
				menuBar()->show();
			}
		}
	}

	return QMainWindow::event(e );
}

QWorkspace* DMdiWindow::workspace()
{
	return m_workspace;
}
