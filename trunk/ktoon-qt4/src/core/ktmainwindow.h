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

#include "dactionmanager.h"
#include "ktviewdocument.h"

#include "ktworkspace.h"

#include "ktexposuresheet.h"
#include "kinaswidget.h"

#include "ktpenwidget.h"

#include "ktpreferences.h"
#include "ktsceneswidget.h"

#include "ktcolorpalette.h"

#include "ktlibrarywidget.h"
#include "kttimeline.h"

#include "ktviewdocument.h"
#include "ktviewcamera.h"

#include "ktgceditor.h"
#include "kthelpwidget.h"

#include "ktexportwidget.h"

#include "dmainwindow.h"
#include "ktstatusbar.h"

#include "ktosd.h"

// Projects
#include "ktprojectmanager.h"
#include "ktsplash.h"

#include "config.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTMainWindow : public DMainWindow
{
	Q_OBJECT
	public:
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
		
		void ui4project(QWidget *widget);
		
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
		void updateOpenRecentMenu(QMenu *menu);
		
	private slots:
		void createNewProject(const QString &name, const QSize &size = QSize(-1,-1), const QString& renderType = tr("Image"), const int fps = 24  );
		void newViewDocument(const QString &name = QString::null);
		void newViewCamera(KTScene *scene = 0);
		void newProject();
		bool closeProject();
		void openProject();
		
		void save();
		void showHelpPage(const QString &title, const QString &document);
		void showWidgetPage();
		
		void showAnimationMenu(const QPoint &p);
		
	private slots:
		void messageToStatus(const QString &);
		void messageToOSD(const QString &, int level = KTOsd::Info);
		void preferences();
		void aboutKToon();
		void showTipDialog();
		void importPalettes();
		void connectToDisplays(const QWidget *widget);
		
		void saveProject();
		void saveProjectAs();
		
		void openRecentProject();
		
		
	private slots: // defined in ktmainwindow-project.cpp
		// Graphic Components
		void rotateCurrentElement(int a);
		void scaleCurrentElement(double dx,double dy);
		void translateCurrentElement(double dx,double dy);
		void shearCurrentElement(double dx,double dy);
		void addCurrentGraphicToLibrary();
		void addGraphicComponent(const AGraphicComponent *graphic);
		
		//Layers
		void insertLayer( const QString &, bool addedToEnd);
		void setLayerVisibilityChanged(int idLayer, bool isVisible);
		void moveLayer(bool up);
		void removeLayer(int );
		void selectLayer(int );
		void setLayerName(int indexLayer, const QString& name);
		
		//Frames
		void insertFrame( const QString &name, bool addedToEnd);
		void selectFrame(int layer, int frame);
		void moveFrame(bool up);
		void removeFrame( );
		void lockFrame();
		void setFrameName(int indexLayer, int indexFrame, const QString& name );
		// Scenes
		
		void insertScene( const QString &name, bool addedToEnd);
		void changeScene(int index);
		
		//others
		void changeCurrentColors(const QBrush &, const QBrush &);
		void changeCurrentPen(const QPen &pen);
		
		void changeFPS(int fps);
		
		void insertImage();
		
	private:
		KTProjectManager *m_projectManager;
		
	private:
		KTWorkspace *m_drawingSpace;
		KTWorkspace *m_animationSpace;
		KTStatusBar *m_statusBar;
		DActionManager *m_actionManager;
		QMenu *m_fileMenu,*m_settingsMenu, *m_viewMenu, *m_insertMenu, *m_toolsMenu, *m_windowMenu,*m_helpMenu;
		
		KTOsd *m_osd;
		
		QStringList m_recentProjects;
		
	// Components
	private:
		KTExposureSheet *m_exposureSheet;
		KTScenesWidget *m_scenes;
		KTTimeLine *m_timeLine;
		KTHelpWidget *m_helper;
		KTLibraryWidget *m_libraryWidget;
		KTColorPalette *m_colorPalette;
		KTPenWidget *m_penWidget;
		KTGCEditor *m_gcEditor;
		
};

#endif
