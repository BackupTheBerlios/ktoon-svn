/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "dbuttonbar.h"
#include "dviewbutton.h"

#include <QToolButton>
#include <QBoxLayout>
#include <QAction>


DButtonBar::DButtonBar(Qt::ToolBarArea area, QWidget *parent) : QToolBar(parent)
{
	setMovable(false);
	
// 	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	
	setIconSize(QSize(16,16));
	
	m_buttons.setExclusive( true );
	
	switch(area)
	{
		case Qt::LeftToolBarArea:
		{
			setWindowTitle(tr("Left Button Bar"));
		}
		break;
		case Qt::RightToolBarArea:
		{
			setWindowTitle(tr("Right Button Bar"));
		}
		break;
		case Qt::TopToolBarArea:
		{
			setWindowTitle(tr("Top Button Bar"));
		}
		break;
		case Qt::BottomToolBarArea:
		{
			setWindowTitle(tr("Bottom Button Bar"));
		}
		break;
		default: break;
	}
}


DButtonBar::~DButtonBar()
{
}

void DButtonBar::addButton(DViewButton *viewButton)
{
	if ( m_buttons.checkedButton() )
	{
		m_buttons.checkedButton()->setChecked(false);
	}
	
	QAction *act = addWidget(viewButton);
	m_buttons.addButton(viewButton);
	
	m_actions[viewButton] = act;
	act->setVisible(true);
	
	if ( !isVisible() ) show();
}

void DButtonBar::removeButton(DViewButton *viewButton)
{
	m_buttons.removeButton(viewButton);
	removeAction( m_actions[viewButton] );
	m_actions.remove(viewButton);
	
	if ( isEmpty() ) hide();
}

bool DButtonBar::isEmpty() const
{
	return m_buttons.buttons().isEmpty();
}


