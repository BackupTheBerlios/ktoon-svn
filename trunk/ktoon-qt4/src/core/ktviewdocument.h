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

#include "ktactionmanager.h"

#include "afilterinterface.h"
#include "atoolinterface.h"

#include "ktpaintareacontainer.h"

#include "ktdocument.h"

#include "ktactionmanager.h"

#include "ktconfigurationarea.h"

/**
 * @author Jorge Cuadrado
*/
class KTViewDocument : public KTMdiWindow
{
	Q_OBJECT
	public:
		KTViewDocument( const QSize& size ,  const QString& projectName , const QString& renderType, KTDocument *doc, QWorkspace *parent = 0 );
		~KTViewDocument();
		void close();
		APaintArea *drawArea();
		
		
	public slots:
		void disablePreviousOnionSkin();
		void onePreviousOnionSkin();
		void twoPreviousOnionSkin();
		void threePreviousOnionSkin();
		void disableNextOnionSkin();
		void oneNextOnionSkin();
		void twoNextOnionSkin();
		void threeNextOnionSkin();
		
		void setScene(KTScene* scene);
		
	private:
		QActionGroup *m_gridGroup, *m_editGroup, *m_editGroup2, *m_viewNextGroup, *m_viewPreviousGroup;
		QMenu *m_brushesMenu, *m_selectionMenu, *m_fillMenu, *m_filterMenu;
		QMenu *m_toolsMenu, *m_editMenu, *m_viewMenu;
		QAction *m_aUndo, *m_aRedo, *m_aClose;
		QToolBar *m_barGrid, *m_toolbar;
		
		KTPaintAreaContainer *m_paintAreaContainer;
		
		KTDocument *m_document;
		QString m_title;
		
	private:
		void createActions();
		void setupViewActions();
		void setupGridActions();
		void setupEditActions();
		void setupEdit2Actions();
		
		void createToolbar();
		void createMenu();
		void createTools();
		
	private: 
		// Plugins
		void loadPlugins();
		
	private:
		QDir m_pluginDirectory;
		KTActionManager *m_actionManager;
		KTConfigurationArea *m_configurationArea;
		
		
	private slots:
		void showPos(const QPoint &p);	
		void setCursor(const QCursor &c);
		
		void selectTool();
		void selectToolFromMenu(QAction *act);
		void applyFilter();
		
	protected:

};

#endif
