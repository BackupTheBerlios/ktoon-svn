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


KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), m_viewLayer(0)
{
	KTINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
	m_imgs <<  
			QPixmap(KTOON_THEME_DIR+"/icons/add_layer.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/remove_layer.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/show_hide_all_layers.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/add_frame.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/remove_frame.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/lock.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/arrowup.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/arrowdown.png" );
	setupButtons();
	m_scenes = new KTTabWidget(this);
	addChild(m_scenes);
	show();
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
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	m_buttonsPanel->setMaximumHeight( m_buttonsPanel->sizeHint().height());

	addChild(m_buttonsPanel);
}

void KTExposureSheet::createLayerManager()
{
	//TODO:kuadrosx crear una clase que me permita visualizar los items y seleccionarlos un QCheckBox
	m_layerManager = new QListView;
	m_layerManager->setMovement ( QListView::Static  );
	m_layerManager->resize(150,100);
}

void KTExposureSheet::addScene(const QString &name)
{
	KTTableExposure *newLayer = new KTTableExposure(100, 1, m_scenes);
	
	m_scenes->addTab(newLayer, name); // TODO: Necesitamos una forma facil de identificar scenas, puede ser por el indice de insersion
	connect(newLayer, SIGNAL(layerVisibilityChanged( int, bool)), this, SIGNAL(layerVisibilityChanged( int, bool)));
	connect(newLayer, SIGNAL(clickedFrame()), this, SIGNAL(frameSelected()));
	connect(newLayer, SIGNAL(cellSelected( int, int )), this, SIGNAL(frameSelected(int, int)));
	connect(newLayer, SIGNAL(layerSelected(int)), this, SIGNAL(layerSelected(int)));
	connect(newLayer, SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame(bool)));
	
	m_viewLayer = newLayer;
}

void KTExposureSheet::renameScene(const QString &name, int id)
{
	m_scenes->setTabText(id, name);
}

void KTExposureSheet::applyAction(int action)
{
	KT_FUNCINFO;
	
	if ( ! m_viewLayer )
	{
		ktFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}
	
	switch(action)
	{
		case InsertLayer:
		{
			
			emit requestInsertLayer();
// 			m_viewLayer->insertLayer(100);
// 			slotInsertLayer();
			break;
		}
		case RemoveLayer:
		{
			emit requestRemoveLayer();
// 			m_viewLayer->removeCurrentLayer();
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
// 			m_viewLayer->setUseFrame();
			m_viewLayer->insertFrames();
// 			emit requestInsertFrame();
			break;
		}
		case RemoveFrame:
		{
// 			m_viewLayer->removeFrame();
			emit requestRemoveFrame();
			break;
		}
		case LockFrame:
		{
			m_viewLayer->lockCurrentFrame();
			break;
		}
		case MoveFrameUp:
		{
			emit requestMoveUpFrame();
// 			m_viewLayer->moveCurrentFrame(KTTableExposure::Up );
			break;
		}
		case MoveFrameDown:
		{
			emit requestMoveDownFrame();
// 			m_viewLayer->moveCurrentFrame(KTTableExposure::Down );
			break;
		}
	}
}

void KTExposureSheet::addFrame(int idLayer, const QString &name, bool addedToEnd)
{
	ktDebug( ) << "KTExposureSheet::addFrame(" << idLayer << " , "<< name << " , "  << addedToEnd << ")" << endl;
// 	m_viewLayer->currentLayerExposure()->setUseFrames(name);
	m_viewLayer->setUseFrame(idLayer, name, addedToEnd);
}

void KTExposureSheet::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

// void KTExposureSheet::loadLayersAndKeyframes( QList<Layer*> layers )
// {
// 	m_viewLayer->loadLayers(layers);
// }

void KTExposureSheet::updateLayersAndKeyframes()
{
	m_viewLayer->updateLayers();
}

void KTExposureSheet::insertLayer(const QString& name)
{
	KT_FUNCINFO;
	//change m_viewLayer for currentTable
	if ( m_viewLayer )
	{
		m_viewLayer->insertLayer(100, name);
	}
	else
	{
		ktFatal() << "KTExposureSheet::insertLayer: No layer view!" << endl;
	}
}

void KTExposureSheet::removeCurrentLayer()
{
	//change m_viewLayer for currentTable
	m_viewLayer->removeCurrentLayer();
}

void KTExposureSheet::removeCurrentFrame()
{
	m_viewLayer->removeFrame();
}

void KTExposureSheet::moveUpFrame()
{
	m_viewLayer->moveCurrentFrame(KTTableExposure::Up );
}

void KTExposureSheet::moveDownFrame()
{
	m_viewLayer->moveCurrentFrame(KTTableExposure::Down );
}

