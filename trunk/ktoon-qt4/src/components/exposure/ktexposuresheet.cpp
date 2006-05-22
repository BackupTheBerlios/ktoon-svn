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
#include "dapplication.h"

#include "ddebug.h"
#include "doptionaldialog.h"

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>


KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), m_currentTable(0)
{
	DINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
	m_imgs <<  
			QPixmap(THEME_DIR+"/icons/add_layer.png" ) <<
			QPixmap(THEME_DIR+"/icons/remove_layer.png" ) <<
			QPixmap(THEME_DIR+"/icons/add_frame.png" ) <<
			QPixmap(THEME_DIR+"/icons/remove_frame.png" ) <<
			QPixmap(THEME_DIR+"/icons/lock.png" ) <<
			QPixmap(THEME_DIR+"/icons/arrowup.png" ) <<
			QPixmap(THEME_DIR+"/icons/arrowdown.png" );
	setupButtons();
	m_scenes = new DTabWidget(this);
	connect( m_scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(m_scenes);
// 	show();
}

KTExposureSheet::~KTExposureSheet()
{
	m_imgs.clear();
	DEND;
}


void KTExposureSheet::setupButtons()
{
	m_buttonsPanel = new QGroupBox(this);
	QHBoxLayout *layout = new QHBoxLayout(m_buttonsPanel);
	layout->setMargin(0);
	layout->setSpacing(0);
	m_buttonGroup = new QButtonGroup(m_buttonsPanel);
	connect(m_buttonGroup, SIGNAL(buttonClicked ( QAbstractButton *)), this, SLOT(actionButton( QAbstractButton *)));
	QStringList toolTips;
	toolTips << tr("Insert Layer") << tr("Remove Layer") << tr("Insert Keyframes") << tr("Remove Keyframe") << tr("Lock Frame") << tr("Move Keyframe Up") << tr("Move Keyframe Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton * tmpButton = new DImageButton(m_imgs[i], 25, m_buttonsPanel);
		m_buttonGroup->addButton (tmpButton);
		tmpButton->setToolTip(toolTips[i]);
		layout->addWidget(tmpButton);
	}
	m_buttonsPanel->setMaximumHeight( m_buttonsPanel->sizeHint().height());

	addChild(m_buttonsPanel);
}


void KTExposureSheet::addScene(const QString &name)
{
	D_FUNCINFO;
	KTTableExposure *newLayer = new KTTableExposure(100, 0);
	
	m_tables << newLayer;
	
	m_scenes->addTab(newLayer, name);
	connect(newLayer, SIGNAL(layerVisibilityChanged( int, bool)), this, SIGNAL(layerVisibilityChanged( int, bool)));
	
	connect(newLayer, SIGNAL(cellSelected( int, int )), this, SIGNAL(frameSelected(int, int)));
	connect(newLayer, SIGNAL(layerSelected(int)), this, SIGNAL(layerSelected(int)));
	
	connect(newLayer, SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame(bool)));
	
	connect(newLayer, SIGNAL(requestCopyFrame(int)), this,   SIGNAL(requestCopyFrame(int)));
	
	connect(newLayer, SIGNAL(requestPasteFrame(int)), this,   SIGNAL(requestPasteFrame(int)));
	
	connect(newLayer, SIGNAL(requestCloneFrame(int, int)), this, SIGNAL(requestCloneFrame(int, int)));
	
	connect(newLayer, SIGNAL(requestRenameLayer(int, const QString&)), this, SIGNAL(requestRenameLayer(int, const QString&)));
	
	connect(newLayer, SIGNAL(requestRenameFrame(int, int, const QString&)), this, SIGNAL(requestRenameFrame(int, int, const QString&)));
	
	m_currentTable = newLayer;
	m_scenes->setCurrentWidget(m_currentTable);
}

void KTExposureSheet::renameScene(const QString &name, int id)
{
	m_scenes->setTabText(id, name);
}


void KTExposureSheet::applyAction(int action)
{
	D_FUNCINFO;
	
// 	dDebug() << m_tables.count();
	
	if ( m_tables.count() == 0 || m_currentTable == 0 )
	{
		dFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
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
			DCONFIG->beginGroup("Exposure");
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskLayer", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this layer?"),tr("Remove?"), this);
		
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskLayer", dialog.shownAgain());
				DCONFIG->sync();
			}
			emit requestRemoveLayer(m_currentTable->currentLayer());
			break;
		}
		case InsertFrames:
		{
			m_currentTable->insertFrames();
			break;
		}
		case RemoveFrame:
		{
			DCONFIG->beginGroup("Exposure");
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskFrame", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this frame?"),tr("Remove?"), this);
		
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskFrame", dialog.shownAgain());
				DCONFIG->sync();
			}
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
// 	dDebug( ) << "KTExposureSheet::addFrame(" << idLayer << " , "<< name << " , "  << addedToEnd << ")" << endl;
	m_currentTable->setUseFrame(idLayer, name, addedToEnd);
}

void KTExposureSheet::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

void KTExposureSheet::setCurrentCell( int idLayer, int idFrame)
{
	if(m_currentTable)
	{
		m_currentTable->setCurrentCell( idLayer, idFrame);
	}
}


void KTExposureSheet::insertLayer(const QString& name)
{
// 	D_FUNCINFO;
// 	dDebug() << "KTExposureSheet::insertLayer(const QString& " << name  << ")";
	if ( m_currentTable )
	{
		m_currentTable->insertLayer(name);
		
	}
	else
	{
		dFatal() << "KTExposureSheet::insertLayer: No layer view!" << endl;
	}
}

void KTExposureSheet::removeLayer(int index)
{
	dDebug() << "removeLayer(int" <<  index << ")";
	if ( m_currentTable )
		m_currentTable->removeLayer( index );
}


void KTExposureSheet::removeCurrentFrame()
{
	if ( m_currentTable )
	m_currentTable->removeFrame();
}

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

void KTExposureSheet::moveLayer(bool left)
{
	D_FUNCINFO;
	m_currentTable->moveCurrentLayer(left);
}


void KTExposureSheet::lockCurrentFrame()
{
	m_currentTable->lockCurrentFrame();
}

void KTExposureSheet::setScene(int index)
{
	D_FUNCINFO;
// 	dDebug() << "KTExposureSheet::setScene(int "<< index << ")";
	if(index != m_scenes->indexOf(m_currentTable) && m_tables.count() >= index)
	{
		m_currentTable = m_tables[index];
		m_scenes->setCurrentWidget(m_tables[index]);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
// 	dDebug() << "KTExposureSheet::emitRequestChangeScene(int" <<  index << ")" << m_scenes->indexOf(m_currentTable);
	
	if(index != m_scenes->indexOf(m_currentTable))
	{
		emit requestChangeScene(index);
	}
}


void KTExposureSheet::closeAllScenes()
{
	D_FUNCINFO;
	
	delete m_currentTable;
	
	m_scenes->removeAllTabs();
	m_tables.clear();
	
	m_currentTable = 0;
}

void KTExposureSheet::setLayer(int index)
{
	m_currentTable->setLayer( index );
}

void KTExposureSheet::setFrameName(int indexLayer, int indexFrame, const QString& name )
{
	dDebug() << "setFrameName(" << indexLayer << "," << indexFrame << "," << name  << ")";
	m_currentTable->setFrameName(indexLayer, indexFrame, name);
}

void KTExposureSheet::setLayerName(int indexLayer, const QString& name )
{
	dDebug() << "setLayerName(" << indexLayer << "," << name << ")";
	m_currentTable->setLayerName( indexLayer, name );
}
