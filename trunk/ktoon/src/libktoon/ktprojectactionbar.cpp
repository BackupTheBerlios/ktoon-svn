/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktprojectactionbar.h"

#include <QVBoxLayout>
#include <QVariant>

#include <ktglobal.h>
#include <ddebug.h>

#include <dseparator.h>
#include <dconfig.h>
#include <doptionaldialog.h>
#include <dimagebutton.h>

KTProjectActionBar::KTProjectActionBar(Actions actions, Qt::Orientation orientation, QWidget *parent) : QWidget(parent ), m_orientation(orientation), m_isAnimated(true)
{
	connect(&m_actions, SIGNAL(buttonClicked(int)), this, SLOT(emitActionSelected(int)));
	
	setup(actions);
	
	setFixedSize( 22 );
}


KTProjectActionBar::~KTProjectActionBar()
{
}


void KTProjectActionBar::setFixedSize(int s)
{
	m_fixedSize = s;
	
// 	switch(m_orientation )
// 	{
// 		case Qt::Horizontal:
// 		{
// 			setMaximumHeight( s );
// 		}
// 		break;
// 		case Qt::Vertical:
// 		{
// 			setMaximumWidth( s );
// 		}
// 		break;
// 	}
	
// 	foreach( QAbstractButton *button, m_actions.buttons() )
// 	{
// 		button->setIconSize(QSize(m_fixedSize, m_fixedSize));
// 	}
}


void KTProjectActionBar::setup(Actions actions)
{
	QBoxLayout *mainLayout = 0;
	
	switch(m_orientation)
	{
		case Qt::Vertical:
		{
			mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
			m_buttonLayout = new QBoxLayout(QBoxLayout::TopToBottom);

		}
		break;
		case Qt::Horizontal:
		{
			mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
			m_buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
			
		}
		break;
	}
	
	mainLayout->setSpacing( 0);
	mainLayout->setMargin( 1);
	
	m_buttonLayout->setSpacing(1  );
	m_buttonLayout->setMargin( 1);
	
	m_buttonLayout->addStretch();
	
	int size = 16;
	
	if ( actions & InsertFrame )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/add_frame.png" ), size);
		button->setToolTip(tr("Insert frame") );
		
		button->setShortcut(QKeySequence(Qt::Key_Plus));
		
		m_actions.addButton(button, InsertFrame);
		
		m_buttonLayout->addWidget( button );
		
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & RemoveFrame )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/remove_frame.png"), size);
		button->setToolTip(tr("Remove the frame"));
		
		m_actions.addButton(button, RemoveFrame);
		
		button->setShortcut(QKeySequence(Qt::Key_Minus));
		
		m_buttonLayout->addWidget( button );
		
		button->setAnimated(m_isAnimated);
	}
	 
	if ( actions & MoveFrameUp )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_frame_up.png"), size);
		button->setToolTip( tr("Move frame up"));
		
		m_actions.addButton(button, MoveFrameUp);
		
		m_buttonLayout->addWidget( button );
		
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & MoveFrameDown )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_frame_down.png"), size);
		button->setToolTip(tr("Move frame down") );
		
		m_actions.addButton( button, MoveFrameDown);
		
		m_buttonLayout->addWidget( button );
		
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & LockFrame )
	{
		DImageButton *button = new DImageButton(QIcon(HOME_DIR+"/themes/default/icons/kilit_pic.png"), size);
		button->setToolTip(tr("Lock frame") );
		
		m_actions.addButton( button, LockFrame);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & InsertLayer )
	{
		DImageButton *button = new DImageButton(QIcon(HOME_DIR+"/themes/default/icons/add_layer.png"), size);
		button->setToolTip(tr("Insert a layer"));
		
		m_actions.addButton(button, InsertLayer);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	 
	if ( actions & RemoveLayer )
	{
		DImageButton *button = new DImageButton(QIcon( HOME_DIR+"/themes/default/icons/remove_layer.png"), size);
		button->setToolTip(tr("Remove the layer"));
		
		m_actions.addButton(button, RemoveLayer);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
		
	}
	 
	if ( actions & MoveLayerUp )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_up.png" ), size);
		
		button->setToolTip(tr("Move layer up")  );
		
		m_actions.addButton(button, MoveLayerUp);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(true);
	}
	 
	if ( actions & MoveLayerDown )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_down.png" ), size);
		
		button->setToolTip( tr("Move layer down"));
		
		m_actions.addButton(button, MoveLayerDown);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & LockLayer )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_down.png" ), 22);
		button->setToolTip(tr("Lock layer") );
		
		m_actions.addButton( button, LockLayer );
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & InsertScene )
	{
		DImageButton *button = new DImageButton(QIcon( HOME_DIR+"/themes/default/icons/add_scene.png"), size);  // TODO
		
		button->setToolTip(tr("Insert a scene"));
		
		m_actions.addButton(button, InsertScene);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	 
	if ( actions & RemoveScene )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/remove_scene.png" ), size);  // TODO
		
		button->setToolTip(tr("Remove the scene"));
		m_actions.addButton(button, RemoveScene);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	 
	if ( actions & MoveSceneUp )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_scene_up.png" ), size);
		
		button->setToolTip(tr("Move scene up")  );
		
		m_actions.addButton(button, MoveSceneUp);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	 
	if ( actions & MoveSceneDown )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_scene_down.png" ), size);
		
		button->setToolTip( tr("Move scene down"));
		
		m_actions.addButton(button, MoveSceneDown);
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	
	if ( actions & LockScene )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/lock_scene.png"), size);
		button->setToolTip(tr("Lock scene") );
		
		m_actions.addButton( button, LockScene );
		
		m_buttonLayout->addWidget( button );
		button->setAnimated(m_isAnimated);
	}
	
	m_buttonLayout->addStretch();
	
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
	mainLayout->addLayout( m_buttonLayout );
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
}


void KTProjectActionBar::insertSeparator(int position)
{
	Qt::Orientation sepOrientation = Qt::Vertical;
	
	switch(m_orientation)
	{
		case Qt::Vertical:
		{
			sepOrientation = Qt::Horizontal;
		}
		break;
		case Qt::Horizontal:
		{
			sepOrientation = Qt::Vertical;
		}
		break;
	}
	
	m_buttonLayout->insertWidget(position+1, new DSeparator(sepOrientation), 1, Qt::AlignCenter);
}


DImageButton *KTProjectActionBar::button(Action action)
{
	return qobject_cast<DImageButton *>(m_actions.button(action));
}

void KTProjectActionBar::emitActionSelected(int action)
{
	switch(action)
	{
		case RemoveFrame:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskFrame", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this frame?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskFrame", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
		case RemoveLayer:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskLayer", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this layer?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskLayer", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
		case RemoveScene:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskScene", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this scene?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskScene", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
	}
	
	emit actionSelected( action );
}

