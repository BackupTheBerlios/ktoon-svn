/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTMAINWINDOW_H
#define KTMAINWINDOW_H

#include <qmainwindow.h>
#include <qworkspace.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>

#include "ktactionmanager.h"
#include "ktviewdocument.h"

#include "ktexposuresheet.h"

#include "dmainwindow.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTMainWindow : public DMainWindow
{
	Q_OBJECT
	public:
		KTMainWindow();
		~KTMainWindow();
		virtual void setPalette(const QPalette &);
		
	private:
		void setupBackground();
		/**
		 * Create the action
		 */
		void setupFileActions();
		
		/**
		 * Setup the actions in the toolbar
		 */
		void setupToolBar();
		
		/**
		 * Setup he actions in the menu
		 */
		void setupMenu();
		
		/**
		 * Setup dialogs
		 */
		void setupDialogs();
		
	protected:
		/**
	 	 *  Event for main window closing control
	 	 *
	 	 * Reimplemented from QWidget.
	 	 * @param close_event The input event
		 */
		void closeEvent( QCloseEvent *event );
		void resizeEvent(QResizeEvent *event);
		
		/**
		 *  Creates the application GUI according to the information of the data classes
		 */
		virtual void createGUI();
		/**
		 *  Updates the open recent menu item names according to the @a recent_names list of file names
		 */
		void updateOpenRecentMenu();
		
	private slots:
		void newDocument();
		
	private:
		QWorkspace *m_workSpace;
		KTActionManager *m_actionManager;
		QPopupMenu *m_fileMenu,*m_editMenu, *m_viewMenu, *m_insertMenu, *m_toolsMenu, *m_windowMenu,*m_helpMenu;
};

#endif
