/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#ifndef KTVIEWDOCUMENT_H
#define KTVIEWDOCUMENT_H

#include "ktmdiwindow.h"
#include "ktdocumentruler.h"
#include "drawingarea.h"
#include "q3action.h"
#include "q3mainwindow.h"
#include <qcursor.h>
//Added by qt3to4:
#include <Q3ActionGroup>
#include <Q3PopupMenu>

/**
 *@author Jorge Cuadrado
*/
class KTViewDocument : public KTMdiWindow
{
	Q_OBJECT
	public:
		KTViewDocument( QWidget *parent = 0, const char *name = 0, Qt::WFlags f = 0);
		~KTViewDocument();
		void close();
	private:
		DrawingArea *m_drawArea;
		QWidget *m_container;
		KTDocumentRuler *m_HRuler;
		KTDocumentRuler *m_VRuler;
		Q3ActionGroup *gridGroup, *editGroup, *editGroup2, *viewNextGroup, *viewPreviousGroup;
		Q3Action *m_aSubGrid, *m_aNtsc, *m_aLightTable,*m_aUndo, *m_aRedo, *m_aClose, *m_aFrontBackGrid;
		Q3PopupMenu *m_menuGrid, *m_menuFile;
		Q3ToolBar *m_barGrid;
	
	private:
		void createActions();
		void createToolbar();
		void createMenu();

	private slots:
		void showPos(QPoint p);	
		void setCursor(QCursor c);
	
	protected:
		virtual QSize sizeHint () const ;
		
		

};

#endif
