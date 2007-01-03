/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                          	   *
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

#include "dtabwidget.h"
#include <QWheelEvent>
#include <QTabBar>

#include "ddebug.h"

DTabWidget::DTabWidget(QWidget *parent) : QTabWidget(parent)
{
}


DTabWidget::~DTabWidget()
{
}

void DTabWidget::removeAllTabs()
{
	int count = this->count();
	for (int i = 0; i < count; i++)
	{
		delete currentWidget();
	}
}

#ifndef QT_NO_WHEELEVENT
void DTabWidget::wheelEvent( QWheelEvent *ev )
{
	QRect rect = tabBar()->rect();
	rect.setWidth( width() );
	
	if ( rect.contains(ev->pos()) )
	{
		wheelMove( ev->delta() );
	}
}

void DTabWidget::wheelMove( int delta )
{
	if ( count() > 1 )
	{
		int current = currentIndex();
		if ( delta < 0 )
		{
			current = (current + 1) % count();
		}
		else 
		{
			current--;
			if ( current < 0 )
				current = count() - 1;
		}
		setCurrentIndex( current );
	}
}

#endif


