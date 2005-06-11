/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuarosxx@gmail.com   *
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
#ifndef KTDIALOGBASE_H
#define KTDIALOGBASE_H

#include <qdockwindow.h>
#include <qlayout.h>
#include <qsizepolicy.h> 
#include <qobjectlist.h>
#include <qevent.h>

/**
@author Jorge Cuadrado
*/
class KTDialogBase : public QDockWindow
{
	Q_OBJECT
	public:
		KTDialogBase(Place p = InDock, QWidget *parent = 0, const char *name = 0);
		~KTDialogBase();
		void addChild(QWidget * child);
		
	private:
		QBoxLayout *container;
		QObjectList *childs;
		bool first;
		
	public slots:
		void hideComponets(bool);
		
	signals:
		void documentModified(bool);
	
	protected:
		virtual void mouseDoubleClickEvent ( QMouseEvent * e );
};

#endif
