/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#ifndef KTDIALOGMENU_H
#define KTDIALOGMENU_H

#include <ktdialogbase.h>
#include <q3dockarea.h>
#include <qtoolbox.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QEvent>
#include <QDropEvent>

#include <QDockWidget>

/**
@author David Cuadrado
*/

class KTDialogMenu : public QDockWidget
{
	public:
		KTDialogMenu(QWidget *parent);
		~KTDialogMenu();
		void addMenuEntry(QWidget * child, const QString &title);
		void mouseDoubleClickEvent(QMouseEvent * e );
		
		void enterEvent(QEvent *e);
		
		void dropEvent(QDropEvent* event);
		void dragEnterEvent( QDragEnterEvent * ) ;
		
	private:
		QToolBox *m_bar;
};

#endif
