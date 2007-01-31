/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado       krawek@gmail.com           *
 *                         Jorge Cuadrado       kuadrosxx@gmail.com        *
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

#include "modulebuttonbar.h"

#include <QToolButton>
#include <QHBoxLayout>

#include <dglobal.h>
#include <ddebug.h>
namespace Base {
ModuleButtonBar::ModuleButtonBar(int buttons, QWidget *parent) : QFrame( parent )
{
	setupButtons( buttons );
	
	setFrameStyle( QFrame::Box | QFrame::Raised );
	
	connect(&m_buttons, SIGNAL(buttonClicked ( QAbstractButton *)), this, SIGNAL(buttonClicked ( QAbstractButton *)));
	connect(&m_buttons, SIGNAL(buttonClicked (int)), this, SIGNAL(buttonClicked (int)));
}


ModuleButtonBar::~ModuleButtonBar()
{
}

void ModuleButtonBar::setupButtons(int buttons)
{
	QHBoxLayout *buttonLayout = new QHBoxLayout(this);
	buttonLayout->setSpacing(0);
	buttonLayout->setMargin(0);
	buttonLayout->addStretch( 2) ;
	
	if ( buttons & Add)
	{
		QToolButton *but1 = new QToolButton;
		
		but1->setStatusTip(tr("Add an item"));
		
		but1->setIcon(QIcon(THEME_DIR+"/icons/plussing.png"));
		buttonLayout->addWidget(but1);
		m_buttons.addButton( but1,Add);
	}
	
	if ( buttons & Del)
	{
		QToolButton *but2 = new QToolButton;
		
		but2->setStatusTip(tr("Remove an item"));
		but2->setIcon(QIcon(THEME_DIR+"/icons/del.png"));
		buttonLayout->addWidget(but2);
		m_buttons.addButton( but2,Del );
	}
	
	if ( buttons & Query)
	{
		QToolButton *but3 = new QToolButton;
		
		but3->setStatusTip(tr("Query an item"));
		but3->setIcon(QIcon(THEME_DIR+"/icons/query.png"));
		buttonLayout->addWidget(but3);
		m_buttons.addButton( but3, Query);
	}
	
	if ( buttons & Modify )
	{
		QToolButton *but4 = new QToolButton;
		
		but4->setStatusTip(tr("Modify an item"));
		but4->setIcon(QIcon(THEME_DIR+"/icons/reload.png"));
		buttonLayout->addWidget(but4);
		m_buttons.addButton( but4, Modify);
	}
	
	
	buttonLayout->addStretch( 2) ;
}
}
