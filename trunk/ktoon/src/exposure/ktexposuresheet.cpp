/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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

KTExposureSheet::KTExposureSheet( QWidget *parent, const char *name)
	: KTDialogBase(QDockWindow::OutsideDock, parent, "Exposure Sheet")
{
	qDebug("[Initializing KTExposureSheet]");
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
	
	m_viewLayer = new KTTableExposure(100, 1,this,"table");
	addChild(m_viewLayer);
// 	repaint();
	
}


KTExposureSheet::~KTExposureSheet()
{
	qDebug("[Destroying KTExposureSheet]");
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
		tmpButton = new QPushButton(m_imgs[i], QString::null, buttonsPanel);
		tmpButton-> setAutoDefault( false );
		tmpButton-> setFlat( true );
		tmpButton-> setFocusPolicy( QWidget::NoFocus );
		tmpButton->adjustSize();
		tmpButton->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	addChild(buttonsPanel);
}

void KTExposureSheet::applyAction(int action)
{
	qDebug( "action" + QString::number(action));
	switch(action)
	{
		case InsertLayer:
		{
			slotInsertLayer();
			break;
		}
		case RemoveLayer:
		{
			break;
		}
		case ShowManageLayer:
		{
			break;
		}
		case InsertFrames:
		{
			m_viewLayer->setUseFrame();
			break;
		}
		case RemoveFrame:
		{
			break;
		}
		case LockFrame:
		{
			break;
		}
		case MoveFrameUp:
		{
			break;
		}
		case MoveFrameDown:
		{
			break;
		}
	}
}
		
void KTExposureSheet::slotInsertLayer()
{
	
}