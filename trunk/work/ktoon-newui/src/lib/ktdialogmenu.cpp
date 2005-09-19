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

#include "ktdialogmenu.h"
#include <qapplication.h>
#include <qcursor.h>

KTDialogMenu::KTDialogMenu(QWidget *parent) : KTDialogBase(parent)
{
	setCaption(tr("Menu"));
	
	m_bar = new QToolBox(this);
	
	m_bar->setMinimumSize(100,100);
	
	addMenuEntry(new QWidget(0), "gsgfas");
	
// 	boxLayout()->addWidget(m_bar);
// 	boxLayout()->setDirection ( QBoxLayout::TopToBottom);
	
	hide();
	
	setAcceptDrops(true);
}


KTDialogMenu::~KTDialogMenu()
{
}

void KTDialogMenu::addMenuEntry(QWidget * child, const QString &title)
{
	m_bar->addItem(child, title);
}

void KTDialogMenu::mouseDoubleClickEvent(QMouseEvent *e)
{
	QWidget *curr = m_bar->currentItem () ;
	if( ! curr )
		return;
	
	m_bar->removeItem(curr);
	
	curr->reparent(parentWidget(), curr->pos(), true);
	
	curr->resize(100,100);
	curr->show();
}

void  KTDialogMenu::enterEvent(QEvent *e)
{
	qDebug("Enterrr");
	
	QWidget *curr = QApplication::widgetAt( QCursor::pos() );
	
	if( !curr)
		return;
	
	if (curr && curr != this && curr->inherits("KTDialogBase") )
	{
		qDebug("I have it");
		curr->reparent(m_bar, curr->pos(), true);
		addMenuEntry(curr, "jejeje");
		m_bar->adjustSize();
	}
}

void KTDialogMenu::dragEnterEvent( QDragEnterEvent * e)
{
	qDebug("Draggg");
}

void KTDialogMenu::dropEvent(QDropEvent* event)
{
	qDebug("droppp");
}
