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

#include <QToolButton>
#include <QVBoxLayout>


#include <ktglobal.h>
#include <ddebug.h>

#include <dseparator.h>

KTProjectActionBar::KTProjectActionBar(Actions actions, Qt::Orientation orientation, QWidget *parent) : QWidget(parent ), m_orientation(orientation)
{
	connect(&m_actions, SIGNAL(buttonClicked(int)), this, SIGNAL(actionSelected(int)));
	
	setup(actions);
	
	setFixedSize( 20 );
}


KTProjectActionBar::~KTProjectActionBar()
{
}


void KTProjectActionBar::setFixedSize(int s)
{
	m_fixedSize = s;
	
	switch(m_orientation )
	{
		case Qt::Horizontal:
		{
			setMaximumHeight( s );
		}
		break;
		case Qt::Vertical:
		{
			setMaximumWidth( s );
		}
		break;
	}
	
	foreach( QAbstractButton *button, m_actions.buttons() )
	{
		button->setIconSize(QSize(m_fixedSize, m_fixedSize));
	}
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
	mainLayout->setMargin( 0);
	
	m_buttonLayout->setSpacing( 0);
	m_buttonLayout->setMargin( 0);
	
	m_buttonLayout->addStretch();
	
	if ( actions & InsertFrame )
	{
		QToolButton *button = new QToolButton;
		button->setIcon(QIcon(THEME_DIR+"/icons/add_frame.png" ));
		button->setText(tr("Insert frame") );
		
		button->setShortcut(QKeySequence(Qt::Key_Plus));
		
		m_actions.addButton(button, InsertFrame);
		
		m_buttonLayout->addWidget( button );
	}
	
	if ( actions & RemoveFrame )
	{
		QToolButton *button = new QToolButton;
		button->setIcon(QIcon(THEME_DIR+"/icons/remove_frame.png"));
		button->setText(tr("Remove frame") );
		
		m_actions.addButton(button, RemoveFrame);
		
		button->setShortcut(QKeySequence(Qt::Key_Minus));
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & MoveFrameUp )
	{
		QToolButton *button = new QToolButton;
		button->setIcon(QIcon());
		button->setText( tr("Move frame up"));
		
		m_actions.addButton(button, MoveFrameUp);
		
		m_buttonLayout->addWidget( button );
	}
	
	if ( actions & MoveFrameDown )
	{
		QToolButton *button = new QToolButton;
		button->setText(tr("Move frame down") );
		button->setIcon(QIcon());
		
		m_actions.addButton( button, MoveFrameDown);
		
		m_buttonLayout->addWidget( button );
	}
	
	if ( actions & InsertLayer )
	{
		QToolButton *button = new QToolButton;
		button->setText( tr("Insert layer") );
		button->setIcon(QIcon(HOME_DIR+"/themes/default/icons/add_layer.png"));
		
		m_actions.addButton(button, InsertLayer);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & RemoveLayer )
	{
		QToolButton *button = new QToolButton;
		button->setText( tr("Remove layer") );
		button->setIcon(QIcon( HOME_DIR+"/themes/default/icons/remove_layer.png"));
		
		m_actions.addButton(button, RemoveLayer);
		
		m_buttonLayout->addWidget( button );
		
	}
	 
	if ( actions & MoveLayerUp )
	{
		QToolButton *button = new QToolButton;
		
		button->setText(tr("Move layer up")  );
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, MoveLayerUp);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & MoveLayerDown )
	{
		QToolButton *button = new QToolButton;
		
		button->setText( tr("Move layer down"));
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, MoveLayerDown);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & InsertScene )
	{
		QToolButton *button = new QToolButton;
		
		button->setText(tr("Insert scene") );
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, InsertScene);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & RemoveScene )
	{
		QToolButton *button = new QToolButton;
		
		button->setText(tr("Remove scene") );
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, RemoveScene);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & MoveSceneUp )
	{
		QToolButton *button = new QToolButton;
		
		button->setText(tr("Move scene up")  );
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, MoveSceneUp);
		
		m_buttonLayout->addWidget( button );
	}
	 
	if ( actions & MoveSceneDown )
	{
		QToolButton *button = new QToolButton;
		
		button->setText( tr("Move scene down"));
		button->setIcon(QIcon( ));
		
		m_actions.addButton(button, MoveSceneDown);
		
		m_buttonLayout->addWidget( button );
	}
	
	m_buttonLayout->addStretch();
	
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
	mainLayout->addLayout( m_buttonLayout );
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
}


void KTProjectActionBar::insertSeparator(int position)
{
	Qt::Orientation sepOrientation;
	
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

