/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#include "ktdialogbase.h"
KTDialogBase::KTDialogBase(Place p, QWidget *parent, const char *name)
 : QDockWindow(p, parent, name)
{
	container = boxLayout();
	setCloseMode(Undocked);
	first = true;
	childs = new QObjectList();
	//container->setResizeMode (  QBoxLayout::Minimum);
	//container->setAutoAdd (true);
	
}


KTDialogBase::~KTDialogBase()
{
	delete childs;
}

void KTDialogBase::addChild(QWidget* child)
{
	childs->append(child);
	container->addWidget(child);
	container->setDirection ( QBoxLayout::TopToBottom);
	container->setMargin(5);
}

void KTDialogBase::hideComponets(bool ok)
{
	for( QObject *o = childs->first(); o; o = childs->next() )
	{
			
		if ( o )
			static_cast<QWidget*>(o)->hide();
	}
	
	/*if(ok)
	{
		QObjectList* const list = this->queryList("QFrame");
		for( QObject *o = list->first(); o; o = list->next() )
		{
			
			if ( o )
				static_cast<QWidget*>(o)->hide();
		}
		delete list;
	}
	else
	{
		QObjectList* const list = this->queryList("QFrame");
		for( QObject *o = list->first(); o; o = list->next() )
		{
			if ( o )
			{
				static_cast<QWidget*>(o)->show();
				static_cast<QWidget*>(o)->setSizePolicy( QSizePolicy::Minimum ,QSizePolicy::Minimum ,false);
				
			}
		}
		delete list;
}*/
	setMinimumWidth( sizeHint().width());
	adjustSize ();
}

void KTDialogBase::mouseDoubleClickEvent(QMouseEvent* e)
{
	qDebug("mouseDoubleClickEvent");
	
	if(first)
	{
		
		hideComponets(true);
		first = false;
		
	}
	else
	{
		hideComponets(false);
		first = true;
        }

}
