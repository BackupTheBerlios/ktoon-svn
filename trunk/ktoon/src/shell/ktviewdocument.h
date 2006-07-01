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

#include "dmdiwindow.h"
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

#include "ktdocument.h"
#include "ktconfigurationarea.h"

#include "ktglobal.h"

/**
 * @author Jorge Cuadrado
*/
class KTViewDocument : public DMdiWindow
{
	Q_OBJECT
	public:
		KTViewDocument( const QSize& size ,  const QString& projectName , KToon::RenderType renderType, KTDocument *doc, QWorkspace *parent = 0 );
		~KTViewDocument();
		void close();
		
		QSize sizeHint() const;
		
		
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
		
		void setScene(KTScene* scene);
		
		void setZoomFactor(int porcent);
		
	private:
		QActionGroup *m_gridGroup, *m_editGroup, *m_viewNextGroup, *m_viewZoomGroup, *m_viewPreviousGroup;
		QMenu *m_brushesMenu, *m_selectionMenu, *m_fillMenu, *m_filterMenu;
		QMenu *m_toolsMenu, *m_editMenu, *m_viewMenu, *m_orderMenu;
		QAction *m_aUndo, *m_aRedo, *m_aClose;
		QToolBar *m_barGrid, *m_toolbar;
		QSpinBox *m_zoomFactorSpin;
		
		KTDocument *m_document;
		QString m_title;
		
	private:
		void createActions();
		void setupViewActions();
		void setupEditActions();
		void setupOrderActions();
		
		void createToolbar();
		void createMenu();
		void createTools();
		
		
	private slots: 
		// Plugins
		void loadPlugins();
		
	private:
		DActionManager *m_actionManager;
		KTConfigurationArea *m_configurationArea;
		
// 		DCommandHistory *m_history;
		
		
	private slots:
		void showPos(const QPoint &p);	
		void setCursor(const QCursor &c);
		
		void updateZoomFactor( double f);
		void configure();
		
		
		void selectToolFromMenu(QAction *act);
		
	public slots:
		void selectTool();
		void applyFilter();
		
	protected:
// 		void closeEvent(QCloseEvent *e);
};

#endif
