/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include <QtGlobal> 
#ifdef Q_WS_X11

#include "dtermtab.h"
#include <qlabel.h>

#include <QTabBar>

#include "dterm.h"

DTermTab::DTermTab(QWidget *parent) : DTabWidget(parent)
{
// 	tabBar()->setShape( QTabBar::TriangularNorth );
	
	m_newTab = new QToolButton(this);
	m_newTab->setText(tr("New"));
	
	connect(m_newTab, SIGNAL(clicked()), this, SLOT(newTerm()));
	
	m_closeTab = new QToolButton(this);
	m_closeTab->setText(tr("Close"));
	
	
	connect(m_closeTab, SIGNAL(clicked()), this, SLOT(closeCurrentTerm()));
	
	setCornerWidget( m_newTab, Qt::TopLeftCorner );
	setCornerWidget( m_closeTab, Qt::TopRightCorner );
	
	m_closeTab->show();
	m_newTab->show();
	
	newTerm();
}


DTermTab::~DTermTab()
{
}

void DTermTab::newTerm()
{
	DTerm *term = new DTerm;
	addTab(term, tr("Console %1").arg(count()));
		
	term->showTerm();
		
	connect(term, SIGNAL(termClosed()), this, SLOT(closeTermTab()));
}

void DTermTab::closeCurrentTerm()
{
	removeTab(currentIndex());
}

void DTermTab::closeTermTab()
{
	DTerm *term = qobject_cast<DTerm *>(sender());
	if( term)
	{
		closeTermTab( term );
	}
}

void DTermTab::closeTermTab(DTerm *term)
{
	removeTab(indexOf(term));
}

#endif // Q_WS_X11
