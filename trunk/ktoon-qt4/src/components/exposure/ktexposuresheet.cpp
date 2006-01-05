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

#include "ktdebug.h"

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>


KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), m_currentTable(0)
{
	KTINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
	m_imgs <<  
			QPixmap(KTOON_THEME_DIR+"/icons/add_layer.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/remove_layer.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/add_frame.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/remove_frame.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/lock.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/arrowup.png" ) <<
			QPixmap(KTOON_THEME_DIR+"/icons/arrowdown.png" );
	setupButtons();
	m_scenes = new KTTabWidget(this);
	connect( m_scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(m_scenes);
	show();
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
	toolTips << tr("Insert Layer") << tr("Remove Layer") << tr("Insert Keyframes") << tr("Remove Keyframe") << tr("Lock Frame") << tr("Move Keyframe Up") << tr("Move Keyframe Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton * tmpButton = new KTImageButton(m_imgs[i], 25, m_buttonsPanel);
		m_buttonGroup->addButton (tmpButton);
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	m_buttonsPanel->setMaximumHeight( m_buttonsPanel->sizeHint().height());

	addChild(m_buttonsPanel);
}


void KTExposureSheet::addScene(const QString &name)
{
	KT_FUNCINFO;
	KTTableExposure *newLayer = new KTTableExposure(100, 1);
	
	m_tables << newLayer;
	
	m_scenes->addTab(newLayer, name);
	connect(newLayer, SIGNAL(layerVisibilityChanged( int, bool)), this, SIGNAL(layerVisibilityChanged( int, bool)));
	
	connect(newLayer, SIGNAL(cellSelected( int, int )), this, SIGNAL(frameSelected(int, int)));
	connect(newLayer, SIGNAL(layerSelected(int)), this, SIGNAL(layerSelected(int)));
	
	connect(newLayer, SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame(bool)));
	
	connect(newLayer, SIGNAL(requestCopyFrame(int)), this,   SIGNAL(requestCopyFrame(int)));
	
	connect(newLayer, SIGNAL(requestPasteFrame(int)), this,   SIGNAL(requestPasteFrame(int)));
	
	m_currentTable = newLayer;
	m_scenes->setCurrentWidget(m_currentTable);
}

void KTExposureSheet::renameScene(const QString &name, int id)
{
	m_scenes->setTabText(id, name);
}


void KTExposureSheet::applyAction(int action)
{
	KT_FUNCINFO;
	
	ktDebug() << m_tables.count();
	
	if ( m_tables.count() == 0 || m_currentTable == 0 )
	{
		ktFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}
	
	switch(action)
	{
		case InsertLayer:
		{
			emit requestInsertLayer();
			emit layerSelected(m_currentTable->currentLayer());
// 			m_currentTable->insertFrames();
			break;
		}
		case RemoveLayer:
		{
			emit requestRemoveLayer();
			break;
		}
		case InsertFrames:
		{
			m_currentTable->insertFrames();
			break;
		}
		case RemoveFrame:
		{
			emit requestRemoveFrame();
			break;
		}
		case LockFrame:
		{
// 			m_currentTable->testLockFrame();
			emit requestLockFrame();
			break;
		}
		case MoveFrameUp:
		{
			emit requestMoveFrame(true);
			break;
		}
		case MoveFrameDown:
		{
			emit requestMoveFrame(false);
			break;
		}
	}
}

void KTExposureSheet::addFrame(int idLayer, const QString &name, bool addedToEnd)
{
	ktDebug( ) << "KTExposureSheet::addFrame(" << idLayer << " , "<< name << " , "  << addedToEnd << ")" << endl;
	m_currentTable->setUseFrame(idLayer, name, addedToEnd);
}

void KTExposureSheet::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

// void KTExposureSheet::loadLayersAndKeyframes( QList<Layer*> layers )
// {
// 	m_currentTable->loadLayers(layers);
// }

void KTExposureSheet::setCurrentCell( int idLayer, int idFrame)
{
	if(m_currentTable)
	{
		m_currentTable->setCurrentCell( idLayer, idFrame);
	}
}

void KTExposureSheet::updateLayersAndKeyframes()
{
	m_currentTable->updateLayers();
}

void KTExposureSheet::insertLayer(const QString& name)
{
	KT_FUNCINFO;
	
	if ( m_currentTable )
	{
		m_currentTable->insertLayer(100, name);
	}
	else
	{
		ktFatal() << "KTExposureSheet::insertLayer: No layer view!" << endl;
	}
}

void KTExposureSheet::removeCurrentLayer()
{
	if ( m_currentTable )
	m_currentTable->removeCurrentLayer();
}

void KTExposureSheet::removeCurrentFrame()
{
	if ( m_currentTable )
	m_currentTable->removeFrame();
}

// void KTExposureSheet::moveUpFrame()
// {
// 	m_currentTable->moveCurrentFrame(KTTableExposure::Up );
// }
// 
// void KTExposureSheet::moveDownFrame()
// {
// 	m_currentTable->moveCurrentFrame(KTTableExposure::Down );
// }

void KTExposureSheet::moveFrame(bool up)
{
	if(up)
	{
		m_currentTable->moveCurrentFrame(KTTableExposure::Up );
	}
	else
	{
		m_currentTable->moveCurrentFrame(KTTableExposure::Down );
	}
}

void KTExposureSheet::lockCurrentFrame()
{
	m_currentTable->lockCurrentFrame();
}

void KTExposureSheet::setScene(int index)
{
	KT_FUNCINFO;
	if(index != m_scenes->indexOf(m_currentTable) && m_tables.count() <= index)
	{
		m_currentTable = m_tables[index];
		m_scenes->setCurrentWidget(m_tables[index]);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
// 	ktDebug() << "KTExposureSheet::emitRequestChangeScene(int" <<  index << ")";
	if(index != m_scenes->indexOf(m_currentTable))
	{
		emit requestChangeScene(index);
	}
}


void KTExposureSheet::closeAllScenes()
{
	KT_FUNCINFO;
	
	delete m_currentTable;
	
	m_scenes->removeAllTabs();
	m_tables.clear();
	
	m_currentTable = 0;
}



