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
#include "ktapplication.h"
#include "status.h"
#include "ktdebug.h"

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>


KTExposureSheet::KTExposureSheet( QWidget *parent)
	: KTModuleWidgetBase(parent, "Exposure Sheet")
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

// 	setResizeEnabled ( true );
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
	m_buttonsPanel = new QGroupBox/*QButtonGroup*/(this);
	QHBoxLayout *layout = new QHBoxLayout(m_buttonsPanel);
	layout->setAutoAdd(true);
	layout->setMargin(0);
	layout->setSpacing(0);
	m_buttonGroup = new QButtonGroup(m_buttonsPanel);
	connect(m_buttonGroup, SIGNAL(buttonClicked ( QAbstractButton *)), this, SLOT(actionButton( QAbstractButton *)));
	QStringList toolTips;
	toolTips << tr("Insert Layer") << tr("Remove Layer") << tr("Manage the layer visibility") << tr("Insert Keyframes") << tr("Remove Keyframe") << tr("Lock Frame") << tr("Move Keyframe Up") << tr("Move Keyframe Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton * tmpButton = new KTImageButton(m_imgs[i], 25, m_buttonsPanel);
		m_buttonGroup->addButton (tmpButton);

// 		tmpButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	m_buttonsPanel->setMaximumHeight( m_buttonsPanel->sizeHint().height());

	addChild(m_buttonsPanel);
}

void KTExposureSheet::createLayerManager()
{
	//FIXME:kuadrosx crear una clase que me permita visualizar los items y seleccionarlos unQCheckBox
	m_layerManager = new QListView/*QListView*/(0/*,"", Qt::WType_Popup*/);
	m_layerManager->setMovement ( QListView::Static  );
	
// 	m_layerManager->setViewMode ( QListView::ListMode );
// 	m_layerManager = new QListWidget/*QListView*/(0/*,"", Qt::WType_Popup*/);
// // 	m_layerManager->setColumnCount(1);
// // 	m_layerManager->addColumn( tr( "Name" ), 105 );
// 	m_layerManager->setFrameStyle( WinPanel|Raised );
// 	QStringList list = m_viewLayer->textHeaders();
// 	QListWidgetItem * item = new QListWidgetItem (m_layerManager );
// 
// 	item->setText(list[0] );
// 	item->setFlags(Qt::ItemIsUserCheckable);
// 	m_layerManager->addItem ( item );
// 	m_layerManager->addItems ( list );
	
// 	for(int i = 0; i < list.count(); i++)
// 	{
// 		ktDebug() << "KTExposureSheet " << i;
// 		QList<QListWidgetItem *> items = m_layerManager->findItems(list[i], Qt::MatchCaseSensitive);
// 		ktDebug() << "KTExposureSheet::items.count() " << items.count();
// 		for(int j = 0; j < items.count(); j++)
// 		{
// 			items[j]->setFlags(Qt::ItemIsUserCheckable);
// 		}
// 	}
// 	QTreeWidgetItem * item = new QTreeWidgetItem(0);
// 	item->setText(0, list[0] );
// 	m_layerManager->setHeaderItem (item )	;
// 			>addTopLevelItem(  item );
	
// 	for(int i = 0; i < list.count(); i++)
// 	{
// 	m_layerManager->addItems ( list );
// 		QCheckListItem *checkItem = new QCheckListItem( m_layerManager, list[i], Q3CheckListItem::CheckBox );
// 		checkItem->setVisible( true );
// 		checkItem->setOn( true );
// 		m_layerManager->insertItem ( checkItem);
// 	}

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
// 			QAbstractButton *p = m_buttonsPanel->find(ShowManageLayer);
			QAbstractButton *p = m_buttonGroup->buttons()[ShowManageLayer];
// 			m_layerManager->move( mapToGlobal( p->geometry().bottomLeft() ) );
			m_layerManager->move(mapToParent(p->geometry().bottomLeft() ) );
			m_layerManager->show();
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

void KTExposureSheet::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

void KTExposureSheet::loadLayersAndKeyframes( QList<Layer*> layers )
{
	ktDebug() << "KTExposureSheet::loadLayersAndKeyframes";
	m_viewLayer->loadLayers(layers);
}

void KTExposureSheet::updateLayersAndKeyframes()
{
	m_viewLayer->updateLayers();
}
