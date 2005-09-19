/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktvbox.h"
#include "ktdebug.h"

#include <qobject.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <Q3Frame>

KTVBox::KTVBox(QWidget *parent, const char *name) : Q3Frame(parent, name)
{
	m_pLayout = new QVBoxLayout(this);
	m_pLayout->setAutoAdd(true);
}


KTVBox::~KTVBox()
{
}

void KTVBox::moveWidgetUp(QWidget *widget)
{
	ktDebug() << "Childs " << children ().count() << endl;
	int position = m_pLayout->findWidget(widget);
	
	
	ktDebug() << "Position: " << position << endl;
	
	if (position > 0 )
	{
		m_pLayout->setAutoAdd(false);
		
		m_pLayout->remove(widget);
		m_pLayout->insertWidget(position-1, widget);
		
		m_pLayout->setAutoAdd(true);
	}
	else
	{
		ktDebug() << "The widget isn't in the layout" << endl;
	}
}

void KTVBox::moveWidgetDown(QWidget *widget)
{
	int position = m_pLayout->findWidget(widget);
	
	if (position >= 0 )
	{
		m_pLayout->setAutoAdd(false);
		
		m_pLayout->remove(widget);
		m_pLayout->insertWidget(position+1, widget);
		
		m_pLayout->setAutoAdd(true);
	}
}

void KTVBox::switchWidgetsPosition(QWidget *widget1, QWidget *widget2)
{
	int position1 = m_pLayout->findWidget(widget1);
	int position2 = m_pLayout->findWidget(widget2);
	
	if (position1 > 0 && position2 > 0 )
	{
		m_pLayout->setAutoAdd(false);
		
		m_pLayout->setAutoAdd(true);
	}
}
