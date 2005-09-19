/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#include <qtooltip.h>
#include <qdockarea.h>
#include <iostream>

#include "ktdebug.h"

#include <qobjectlist.h>

KTDialogBase::KTDialogBase(QWidget *parent, const char *name) : KTVBox( parent, name), m_isChildHidden(false)
{
	childs = new QObjectList();
	
	m_title = new KTDialogTitle("the title", this, "DialogTitle");
	
	QToolTip::add(m_title, tr("Double click for roll up"));
	
	connect(m_title, SIGNAL(doubleClicked()), SLOT(toggleView()));
	
// 	connect(this, SIGNAL(placeChanged(QDockWindow::Place)), SLOT(fixPosition(QDockWindow::Place)));
	
// 	setOrientation( Qt::Vertical );
	
	adjustSize();
	hide();
}


KTDialogBase::~KTDialogBase()
{
	delete childs;
}


void KTDialogBase::addChild(QWidget* child)
{
	childs->append(child);
	container->addWidget(child);
}

void KTDialogBase::toggleView()
{
	m_title->setMinimumWidth(m_title->width());
	for( QObject *o = childs->first(); o; o = childs->next() )
	{
		if ( o && ! m_isChildHidden )
			static_cast<QWidget*>(o)->hide();
		else if ( o && m_isChildHidden )
			static_cast<QWidget*>(o)->show();
	}
	
	m_isChildHidden = !m_isChildHidden;
	
	adjustSize ();
}

void KTDialogBase::setCaption(const QString &text)
{
// 	QDockWindow::setCaption(text);
 	m_title->setText(text);
}

void KTDialogBase::setFont( const QFont &f)
{
	KTVBox::setFont(f);
	adjustSize();
}

bool KTDialogBase::event( QEvent * e )
{
	if ( e->type() == QEvent::Hide )
	{
		emit activate(false);
	}
	else if ( e->type() == QEvent::Show )
	{
		adjustSize();
		emit activate(true);
	}

	return KTVBox::event(e );
}

