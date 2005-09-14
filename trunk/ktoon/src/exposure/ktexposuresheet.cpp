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
#include "ktapplication.h"
#include <qlistview.h>
#include "status.h"
#include <ktdebug.h>


KTExposureSheet::KTExposureSheet( QWidget *parent, const char *name)
	: KTDialogBase(QDockWindow::OutsideDock, parent, "Exposure Sheet")
{
	KTINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
	m_imgs <<  
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/layer+.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/layer-.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/show_hide_all_layers.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/frame+.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/frame-.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/lock.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/arrowup.xpm" ) <<
		QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/arrowdown.xpm" );
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
	buttonsPanel = new QHButtonGroup (this);
	connect(buttonsPanel, SIGNAL(clicked(int )), this, SLOT(applyAction(int)));
	QStringList toolTips;
	toolTips << tr("Insert Layer") << tr("Remove Layer") << tr("Manage the layer visibility") << tr("Insert Keyframes") << tr("Remove Keyframe") << tr("Lock Frame") << tr("Move Keyframe Up") << tr("Move Keyframe Down");
	
	for(int i = 0; i < 8; i++)
	{
		QPushButton *tmpButton;
// 		tmpButton = new KTImageButton(m_imgs[i], 25, buttonsPanel);
		tmpButton = new QPushButton(m_imgs[i], QString::null, buttonsPanel);
		tmpButton-> setAutoDefault( false );
		tmpButton-> setFlat( true );
		tmpButton-> setFocusPolicy( QWidget::NoFocus );
		tmpButton->adjustSize();
		tmpButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	buttonsPanel->setMaximumHeight( buttonsPanel->sizeHint().height());
	addChild(buttonsPanel);
}

void KTExposureSheet::createLayerManager()
{
	m_layerManager = new QListView(0,"", WType_Popup);
	
	m_layerManager->addColumn( tr( "Name" ), 105 );
	m_layerManager->setFrameStyle( WinPanel|Raised );
	QStringList list = m_viewLayer->textHeaders();
	
	for(int i = 0; i < list.count(); i++)
	{
		QCheckListItem *checkItem = new QCheckListItem( m_layerManager, list[i], QCheckListItem::CheckBox );
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
			QButton *p = buttonsPanel->find(ShowManageLayer);
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

void KTExposureSheet::loadLayersAndKeyframes( QPtrList<Layer> layers )
{
	ktDebug() << "KTExposureSheet::loadLayersAndKeyframes";
	m_viewLayer->loadLayers(layers);
	
// 	m_viewLayer->touchFirstFrame();
// #if 0
// 	layer_max_value = 0;
// 	number_of_layers = 0;
// 	delete first_layer;
// 	list_of_layers.setAutoDelete( true );
// 	list_of_layers.clear();
// 	list_of_layers.setAutoDelete( false );
// 	visibility_list -> clear();
// 
// 	Layer *l_it;
// 	for ( l_it = layers.first(); l_it; l_it = layers.next() )
// 	{
// 		layer_max_value++;
// 		QPtrList<ESFrame> frame_list;
// 
// 		ESLayer *new_layer = new ESLayer( l_it -> nameLayer(), scroll_area );
// 		if ( l_it == layers.getFirst() )
// 		{
// 			new_layer->move( remove_layer -> x(), 10 );
// 			first_layer = new_layer;
// 			current_layer = new_layer;
// 		}
// 		else
// 			new_layer -> move( last_layer -> x() + last_layer -> width(), last_layer -> y() );
// 		last_layer = new_layer;
// 		connect( new_layer, SIGNAL( clicked() ), SLOT( slotSelectLayer() ) );
// 		connect( new_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
// 		ILayer *new_layer_obj = new ILayer( 0, new_layer );
// 	// 	try { // bullshit, QPtrList never throw exceptions...
// 		list_of_layers.append( new_layer_obj );
// // 	}
// // 	catch(...)
// // 	    {
// // 	    delete new_layer_obj;
// // 	    throw;
// // 	    }
// 		number_of_layers++;
// 
// 		QCheckListItem *new_layer_v = new QCheckListItem( visibility_list, visibility_list -> lastItem(), l_it -> nameLayer(), QCheckListItem::CheckBox );
// 		new_layer_v -> setOn( true );
// 
// 		for ( int i = 1; i <= MAX_NUMBER_OF_FRAMES; i++ )
// 		{
// 			ESFrame *next_frame = new ESFrame( "", 1,scroll_area  );
// 			if ( i == 1 && l_it == layers.getFirst() )
// 				f_frame = next_frame;
// 			next_frame -> move( new_layer -> x(), new_layer -> y() + new_layer -> height() * i );
// 			connect( next_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
// 			connect( next_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
// 			frame_list.append( next_frame );
// 		}
// 
// 		new_layer_obj -> setAvailableFrames( frame_list );
// 
// 	//Load Keyframes
// 		QPtrList<KeyFrame> keyframes = l_it -> keyFrames();
// 		ESFrame *f_it;
// 		KeyFrame *k_it;
// 		for ( k_it = keyframes.first(), f_it = frame_list.first(); k_it; k_it = keyframes.next(), f_it = frame_list.next() )
// 		{
// 			new_layer_obj -> addFrame();
// 			new_layer_obj -> setLastFrame( f_it );
// 			f_it -> setName( k_it -> nameKeyFrame() );
// 			f_it -> setUsed( true );
// 			f_it -> setMotion( k_it -> motionKeyFrame() );
// 		}
// 	}
// 	visibility_list -> show();
// 	visibility_list -> hide();
// #endif
}
