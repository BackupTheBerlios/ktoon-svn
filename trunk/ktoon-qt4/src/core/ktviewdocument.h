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
#include <QCursor>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QMenu>
#include <QDir>
#include <QPluginLoader>

#include "afilterinterface.h"
#include "atoolinterface.h"

#include "ktpaintareacontainer.h"

/**
 * @author Jorge Cuadrado
*/
class KTViewDocument : public KTMdiWindow
{
	Q_OBJECT
	public:
		KTViewDocument( QWidget *parent = 0 );
		~KTViewDocument();
		void close();
		APaintArea *drawArea();
		
	private:
		QActionGroup *gridGroup, *editGroup, *editGroup2, *viewNextGroup, *viewPreviousGroup;
		QMenu *m_brushesMenu, *m_filterMenu;
		
		QMenu *m_toolsSelection, *m_toolsDraw, *m_toolsFills, *m_toolsErasers, *m_toolsView, *m_toolsOrder,*m_toolsAlign, *m_toolsTransform;
		QAction *m_aSubGrid, *m_aNtsc, *m_aLightTable,*m_aUndo, *m_aRedo, *m_aClose, *m_aFrontBackGrid;
		QToolBar *m_barGrid, *m_toolbar;
		
		KTPaintAreaContainer *m_paintAreaContainer;
	private:
		void createActions();
		void createToolbar();
		void createMenu();
		void createTools();
		
	private: 
		// Plugins
		void loadPlugins();
		
	private:
		QDir m_pluginDirectory;

	private slots:
		void showPos(const QPoint &p);	
		void setCursor(const QCursor &c);
		void changeTool(QAction *a);
		
		void selectTool();
		void applyFilter();
		
	protected:

};

#endif
