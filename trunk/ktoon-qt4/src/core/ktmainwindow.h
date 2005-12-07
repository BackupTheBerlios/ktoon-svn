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
#include <QWorkspace>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include <QResizeEvent>
#include <QCloseEvent>

#include "ktactionmanager.h"
#include "ktviewdocument.h"


#include "ktexposuresheet.h"
#include "ktoonscript.h"

#include "preferences.h"
#include "ktsceneswidget.h"

#include "ktbrushwidget.h"
// #include "brushes.h"

// #include "colorpalette.h"
#include "ktcolorpalette.h"

#include "ktlibrarywidget.h"
#include "kttimeline.h"

#include "ktviewdocument.h"
#include "ktviewcamera.h"

#include "ktgceditor.h"
#include "kthelpwidget.h"

#include "dmainwindow.h"
#include "ktstatusbar.h"

#include "ktosd.h"

// Projects
#include "ktprojectmanager.h"

#include "ktsplash.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTMainWindow : public DMainWindow
{
	Q_OBJECT
	public:
		KTMainWindow(KTSplash *splash);
		~KTMainWindow();
		virtual void setPalette(const QPalette &);
		
	private:
		void setupBackground();
		/**
		 * Create the file action 
		 */
		void setupFileActions();
		void setupEditActions();
		
		/**
		 * Create the proyect action
		 */
		void setupProjectActions();
		
		/**
		 * Setup the actions in the toolbar
		 */
		void setupToolBar();
		
		/**
		 * Setup he actions in the menu
		 */
		void setupMenu();
		
		void setupHelpActions();
		
		void ui4project(QWidget *widget);
		
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
		void createNewProject(const QString &name, const QSize &size = QSize(-1,-1) );
		void newViewDocument(const QString &name = QString::null );
		void newProject();
		void closeProject();
		void openProject();
		
		void save();
		void showHelpPage(const QString &title, const QString &document);
		
	private slots:
		void messageToStatus(const QString &);
		void messageToOSD(const QString &);
		void preferences();
		void aboutKToon();
		
	private slots:
		// Graphic Components
		void rotateCurrentElement(int a);
		void scaleCurrentElement(double dx,double dy);
		void translateCurrentElement(double dx,double dy);
		void shearCurrentElement(double dx,double dy);
		
		//Layers
		void insertLayer( const QString &, bool);
		void setLayerVisibilityChanged(int idLayer, bool isVisible);
		void removeLayer(int );
		
		//Frames
		void insertFrame( const QString &name, bool);
		void selectFrame(int layer, int frame);
		void moveFrame(bool up);
		void removeFrame( );
		void lockFrame();
		// Scenes
		
		void insertScene( const QString &name, bool);
		void changeScene(int index);
		
		//others
		void changeCurrentColors(const QBrush &, const QBrush &);
		void changeCurrentBrush(const QPainterPath &form, int thickness);
		
		
		void changeFPS(int fps);
		
	private:
		KTProjectManager *m_projectManager;
		
	private:
		QWorkspace *m_drawingSpace;
		QWorkspace *m_animationSpace;
		KTStatusBar *m_statusBar;
		KTActionManager *m_actionManager;
		QMenu *m_fileMenu,*m_editMenu, *m_proyectMenu, *m_viewMenu, *m_insertMenu, *m_toolsMenu, *m_windowMenu,*m_helpMenu;
		
		KTViewCamera *m_viewCamera;
		
		KTOsd *m_osd;
		
		// Components
	private:
		KTExposureSheet *m_exposureSheet;
		KTScenesWidget *m_scenes;
		KTTimeLine *m_timeLine;
		KTHelpWidget *m_helper;
};

#endif
