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

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QUndoStack>
#include "dactionmanager.h"
#include "ktviewdocument.h"

#include "ktworkspace.h"
#include "ktpreferences.h"

// modules
#include "ktexposuresheet.h"
#include "kinaswidget.h"
#include "ktpenwidget.h"
#include "ktcamerawidget.h"
#include "ktcolorpalette.h"
#include "ktsceneswidget.h"
#include "ktlibrarywidget.h"
#include "kttimeline.h"
#include "ktviewcamera.h"
#include "kthelpwidget.h"
#include "ktexportwidget.h"


#include "ktviewdocument.h"

#include "dtabbedmainwindow.h"
#include "ktstatusbar.h"

#include "dosd.h"

// Projects
#include "ktprojectmanager.h"
#include "ktsplash.h"

#include "ktviewdocument.h"

class KTProjectManagerParams;


/**
 * Ventana principal de la aplicación
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTMainWindow : public DTabbedMainWindow
{
	Q_OBJECT;
	public:
		enum Perspective
		{
			Drawing = 0x01,
			Animation = 0x02,
			All = Drawing | Animation
		};
		
		KTMainWindow(KTSplash *splash = 0);
		~KTMainWindow();
		
		void openProject(const QString &path);
		
	private:
		/**
		 * Create the file action 
		 */
		void setupFileActions();
		void setupSettingsActions();
		void setupWindowActions();
		void setupInsertActions();
		
		/**
		 * Setup the actions in the toolbar
		 */
		void setupToolBar();
		
		/**
		 * Setup he actions in the menu
		 */
		void setupMenu();
		
		void setupHelpActions();
		void setupActions();
		
		void ui4project(QWidget *widget);
		void ui4paintArea(QWidget *widget);
		
	protected:
		/**
	 	 *  Event for main window closing control
	 	 *
	 	 * Reimplemented from QWidget.
	 	 * @param close_event The input event
		 */
		void closeEvent( QCloseEvent *event );
		
		/**
		 *  Creates the application GUI according to the information of the data classes
		 */
		virtual void createGUI();
		/**
		 *  Updates the open recent menu item names according to the @a recent_names list of file names
		 */
		void updateOpenRecentMenu(QMenu *menu, QStringList recents);
		
	private slots:
		void createNewProject( KTProjectManagerParams *params );
		void newViewDocument(const QString &name = QString::null);
		void newProject();
		bool closeProject();
		void openProject();
		
		void save();
		void saveAs();
		
		void showHelpPage(const QString &title, const QString &document);
		void showWidgetPage();
		
		void showAnimationMenu(const QPoint &p);
		
		void changePerspective(QAction *a);
		
	private slots:
		void messageToStatus(const QString &);
		void preferences();
		void aboutKToon();
		void showTipDialog();
		void importPalettes();
		void connectToDisplays(const QWidget *widget);
		
		void saveProject();
		
		void openRecentProject();
		
		void createCommand(const KTPaintAreaEvent *event);
		
	private:
		KTProjectManager *m_projectManager;
		KToon::RenderType m_renderType;
		QString m_fileName;
		
	private:
		KTViewDocument *m_viewDoc;
		KTWorkspace *m_animationSpace;
		KTStatusBar *m_statusBar;
		DActionManager *m_actionManager;
		QMenu *m_fileMenu,*m_settingsMenu, *m_viewMenu, *m_insertMenu, *m_toolsMenu, *m_windowMenu,*m_helpMenu;
		
		QStringList m_recentProjects;
		
		QMenu *m_recentProjectsMenu;
		
	// Components
	private:
		KTExposureSheet *m_exposureSheet;
		KTScenesWidget *m_scenes;
		KTTimeLine *m_timeLine;
		KTHelpWidget *m_helper;
		KTLibraryWidget *m_libraryWidget;
		KTColorPalette *m_colorPalette;
		KTPenWidget *m_penWidget;
		KTCameraWidget *m_cameraWidget;
};

#endif
