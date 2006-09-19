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

#ifndef KTVIEWAREA_H
#define KTVIEWAREA_H

// #include "dmdiwindow.h"
#include "ktdocumentruler.h"
#include <QCursor>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QMenu>
#include <QDir>
#include <QPluginLoader>
#include <QSpinBox>

#include "dactionmanager.h"
#include "ktfilterinterface.h"
#include "kttoolinterface.h"

#include "ktconfigurationarea.h"
#include <QMainWindow>
#include "ktglobal.h"

#include <dosd.h>

class KTPaintArea;
class KTProjectEvent;
class KTProject;
class KTBrushManager;
class KTPaintAreaCommand;
class KTPaintAreaEvent;

/**
 * @author Jorge Cuadrado
*/

class KTViewDocument : public QMainWindow
{
	Q_OBJECT;
	
	public:
		KTViewDocument(KTProject *project, QWidget *parent = 0 );
		~KTViewDocument();
		void close();
		QSize sizeHint() const;
		
		void setAntialiasing(bool useIt );
		void setOpenGL(bool useIt);
		void setDrawGrid(bool draw);
		
		void setRotationAngle(int angle);
		
		KTBrushManager *brushManager() const;
		
		KTPaintAreaCommand *createCommand(const KTPaintAreaEvent *event);
		
		
	public slots:
		void disablePreviousOnionSkin();
		void onePreviousOnionSkin();
		void twoPreviousOnionSkin();
		void threePreviousOnionSkin();
		void disableNextOnionSkin();
		void oneNextOnionSkin();
		void twoNextOnionSkin();
		void threeNextOnionSkin();
		
		void setNextOnionSkin(int n);
		void setPreviousOnionSkin(int n);
		void toggleShowGrid();
		
// 		void setScene(KTScene* scene);
		
		void setZoomFactor(int porcent);
		
	private:
		QActionGroup *m_gridGroup, *m_editGroup, *m_viewNextGroup, *m_viewZoomGroup, *m_viewPreviousGroup;
		QMenu *m_brushesMenu, *m_selectionMenu, *m_fillMenu, *m_filterMenu, *m_viewToolMenu;
		QMenu *m_toolsMenu, *m_editMenu, *m_viewMenu, *m_orderMenu;
		QAction *m_aUndo, *m_aRedo, *m_aClose;
		QToolBar *m_barGrid, *m_toolbar;
		QSpinBox *m_zoomFactorSpin;
		
		KTPaintArea *m_paintArea;
		
	private:
		void createActions();
		void setupViewActions();
		void setupEditActions();
		void setupOrderActions();
		
		void createToolBar();
		void createMenu();
		void createTools();
		
		
	private slots: 
		// Plugins
		void loadPlugins();
		
	private:
		DActionManager *m_actionManager;
		KTConfigurationArea *m_configurationArea;
		
		
	private slots:
		void showPos(const QPointF &p);	
		void setCursor(const QCursor &c);
		
		void updateZoomFactor( double f);
// 		void configure();
		
		
		void selectToolFromMenu(QAction *act);
		
	public slots:
		void selectTool();
		void applyFilter();
		
		bool handleProjectEvent(KTProjectEvent *event);
		
	signals:
		void sendToStatus(const QString& msg);
		void sendToOSD(const QString &msg, DOsd::Level );
		void eventTriggered(const KTProjectEvent *event);
		
	protected:
// 		void closeEvent(QCloseEvent *e);
};

#endif
