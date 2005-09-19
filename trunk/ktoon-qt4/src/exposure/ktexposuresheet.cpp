/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktexposuresheet.h"
#include <qtooltip.h>
//Added by qt3to4:
#include <QPixmap>
#include <QHBoxLayout>
#include <Q3PtrList>
#include "ktapplication.h"
#include <q3listview.h>
#include "status.h"
#include <ktdebug.h>


KTExposureSheet::KTExposureSheet( QWidget *parent, const char *name)
	: KTDialogBase(Q3DockWindow::OutsideDock, parent, "Exposure Sheet")
{
	KTINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
	m_imgs <<  
		QPixmap(KTOON_HOME+"/images/icons/layer+.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/layer-.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/show_hide_all_layers.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/frame+.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/frame-.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/lock.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/arrowup.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/arrowdown.xpm" );
	setupButtons();
	
	m_viewLayer = new KTTableExposure(100, 1,this,"KTExposureTable");
	m_viewLayer->touchFirstFrame();
	addChild(m_viewLayer);
//  	hide();
	show();

	setResizeEnabled ( true );
	connect(m_viewLayer, SIGNAL(clickedFrame()), this, SIGNAL(frameSelected()));
	connect(m_viewLayer, SIGNAL(layerSelected(int)), this, SIGNAL(layerSelected(int)));
	connect(m_viewLayer, SIGNAL(layerInserted()), this, SIGNAL(layerInserted()));
	connect(m_viewLayer, SIGNAL(layerRemoved()), this, SIGNAL(layerRemoved()));
	createLayerManager();
}


KTExposureSheet::~KTExposureSheet()
{
	m_imgs.clear();
	KTEND;
}


void KTExposureSheet::setupButtons()
{
	buttonsPanel = new Q3ButtonGroup(this);
	QHBoxLayout *layout = new QHBoxLayout(buttonsPanel);
	layout->setAutoAdd(true);
	connect(buttonsPanel, SIGNAL(clicked(int )), this, SLOT(applyAction(int)));
	QStringList toolTips;
	toolTips << tr("Insert Layer") << tr("Remove Layer") << tr("Manage the layer visibility") << tr("Insert Keyframes") << tr("Remove Keyframe") << tr("Lock Frame") << tr("Move Keyframe Up") << tr("Move Keyframe Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton *tmpButton;
// 		tmpButton = new KTImageButton(m_imgs[i], 25, buttonsPanel);
		tmpButton = new QPushButton(m_imgs[i], QString::null, buttonsPanel);
		tmpButton-> setAutoDefault( false );
		tmpButton-> setFlat( true );
		tmpButton-> setFocusPolicy( Qt::NoFocus );
		tmpButton->adjustSize();
		tmpButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	buttonsPanel->setMaximumHeight( buttonsPanel->sizeHint().height());
	addChild(buttonsPanel);
}

void KTExposureSheet::createLayerManager()
{
	m_layerManager = new Q3ListView(0,"", Qt::WType_Popup);
	
	m_layerManager->addColumn( tr( "Name" ), 105 );
	m_layerManager->setFrameStyle( WinPanel|Raised );
	QStringList list = m_viewLayer->textHeaders();
	
	for(int i = 0; i < list.count(); i++)
	{
		Q3CheckListItem *checkItem = new Q3CheckListItem( m_layerManager, list[i], Q3CheckListItem::CheckBox );
		checkItem->setVisible( true );
		checkItem->setOn( true );
		m_layerManager->insertItem ( checkItem);
	}

	m_layerManager->resize(150,100);
}

void KTExposureSheet::applyAction(int action)
{
// 	ktDebug(1) << action;
	switch(action)
	{
		case InsertLayer:
		{
			m_viewLayer->insertLayer(100);
// 			slotInsertLayer();
			break;
		}
		case RemoveLayer:
		{
			m_viewLayer->removeCurrentLayer();
			break;
		}
		case ShowManageLayer:
		{
			QAbstractButton *p = buttonsPanel->find(ShowManageLayer);
			m_layerManager->move( mapToGlobal( p->geometry().bottomLeft() ) );
// 			m_layerManager->move(mapToParent(p->geometry().bottomLeft() ) );
// 			m_layerManager->show();
			break;
		}
		case InsertFrames:
		{
			m_viewLayer->setUseFrame();
			break;
		}
		case RemoveFrame:
		{
			m_viewLayer->removeFrame();
			break;
		}
		case LockFrame:
		{
			m_viewLayer->lockCurrentFrame();
			break;
		}
		case MoveFrameUp:
		{
			m_viewLayer->moveCurrentFrame(KTTableExposure::Up );
			break;
		}
		case MoveFrameDown:
		{
			m_viewLayer->moveCurrentFrame(KTTableExposure::Down );
			break;
		}
	}
}

void KTExposureSheet::loadLayersAndKeyframes( Q3PtrList<Layer> layers )
{
	ktDebug() << "KTExposureSheet::loadLayersAndKeyframes";
	m_viewLayer->loadLayers(layers);
}

void KTExposureSheet::updateLayersAndKeyframes()
{
	m_viewLayer->updateLayers();
}