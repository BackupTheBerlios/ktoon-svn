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

#include "ktelabel.h"
//Added by qt3to4:
#include <QMouseEvent>

KTELabel::KTELabel(const QString &text, QWidget *parent) : KTSqueezeLabel(text, parent)
{
	m_editor = new QLineEdit(text, this);
	m_editor->setFont( QFont( font().family(), 9 ) );
	m_editor->hide();
	
	m_editor->setFocusPolicy(Qt::ClickFocus);
	
	connect( m_editor, SIGNAL( returnPressed() ), SLOT( applyText() ) );
	connect( m_editor, SIGNAL( lostFocus() ), SLOT( applyText() ) );
}


KTELabel::~KTELabel()
{
}

void KTELabel::mouseDoubleClickEvent( QMouseEvent *event )
{
	if ( event->button() == Qt::LeftButton )
	{
		rename();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void KTELabel::applyText()
{
	if(completeText() != m_editor->text())
	{
		setText( m_editor -> text() );
		emit renamed( m_editor->text() );
	}
	
	m_editor->hide();
}

void KTELabel::rename()
{
	m_editor->setText( completeText() );
	m_editor->selectAll();
	m_editor->resize( size() );
	m_editor->show();
	m_editor->setFocus();
}

void KTELabel::clearFocus()
{
	m_editor->clearFocus();
}
