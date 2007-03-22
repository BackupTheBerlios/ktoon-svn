/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "delabel.h"
#include <QMouseEvent>

DELabel::DELabel(const QString &text, QWidget *parent) : DSqueezeLabel(text, parent)
{
	m_editor = new QLineEdit(text, this);
	m_editor->setFont( QFont( font().family(), 9 ) );
	m_editor->hide();
	
	m_editor->setFocusPolicy(Qt::ClickFocus);
	
	connect( m_editor, SIGNAL( returnPressed() ), SLOT( applyText() ) );
	connect( m_editor, SIGNAL( lostFocus() ), SLOT( applyText() ) );
}


DELabel::~DELabel()
{
}

void DELabel::mouseDoubleClickEvent( QMouseEvent *event )
{
	if ( event->button() == Qt::LeftButton )
	{
		edit();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void DELabel::applyText()
{
	if(completeText() != m_editor->text())
	{
		setText( m_editor -> text() );
		emit edited( m_editor->text() );
	}
	
	m_editor->hide();
}

void DELabel::edit()
{
	m_editor->setText( completeText() );
	m_editor->selectAll();
	m_editor->resize( size() );
	m_editor->show();
	m_editor->setFocus();
}

void DELabel::clearFocus()
{
	m_editor->clearFocus();
}

void DELabel::setValidator(const QValidator * v)
{
	m_editor->setValidator(v);
}

