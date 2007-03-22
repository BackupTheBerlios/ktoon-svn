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

#include "dvhbox.h"
#include "ddebug.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPoint>

DVHBox::DVHBox(QWidget *parent, Qt::Orientation o) : QFrame(parent)
{
	if ( o == Qt::Vertical )
	{
		m_pLayout = new QVBoxLayout(this);
	}
	else
	{
		m_pLayout = new QHBoxLayout(this);
	}
	
	m_pLayout->setMargin(1);
	m_pLayout->setSpacing(1);
}

DVHBox::DVHBox(QWidget *parent, bool isVertical) : QFrame(parent)
{
	if ( isVertical )
	{
		m_pLayout = new QVBoxLayout(this);
	}
	else
	{
		m_pLayout = new QHBoxLayout(this);
	}
	
	m_pLayout->setMargin(1);
	m_pLayout->setSpacing(1);
}


DVHBox::~DVHBox()
{
}

void DVHBox::addWidget(QWidget *child, Qt::Alignment alignment)
{
	child->setParent(this);
	m_pLayout->addWidget(child);
	if ( alignment != 0 )
	{
		m_pLayout->setAlignment(child, alignment);
	}
}

void DVHBox::moveWidgetUp(QWidget *widget)
{
// 	dDebug() << "Childs " << children ().count() << endl;
	int position = m_pLayout->indexOf(widget);
	
	
// 	dDebug() << "Position: " << position << endl;
	
	if (position > 0 )
	{
		m_pLayout->removeWidget(widget);
		m_pLayout->insertWidget(position-1, widget);
	}
	else
	{
		dError() << "The widget isn't in the layout" << endl;
	}
}

void DVHBox::moveWidgetDown(QWidget *widget)
{
	int position = m_pLayout->indexOf(widget);
	
	if (position >= 0 )
	{
		m_pLayout->removeWidget(widget);
		m_pLayout->insertWidget(position+1, widget);
	}
}

bool DVHBox::event( QEvent* ev )
{
	switch ( ev->type() )
	{
		case QEvent::ChildAdded:
		{
			QChildEvent* childEv = static_cast<QChildEvent *>( ev );
			if ( childEv->child()->isWidgetType() ) 
			{
				QWidget* w = static_cast<QWidget *>( childEv->child() );
				static_cast<QBoxLayout *>( layout() )->addWidget( w );
			}
			return QWidget::event( ev );
		}
		case QEvent::ChildRemoved:
		{
			QChildEvent* childEv = static_cast<QChildEvent *>( ev );
			if ( childEv->child()->isWidgetType() ) 
			{
				QWidget* w = static_cast<QWidget *>( childEv->child() );
				static_cast<QBoxLayout *>( layout() )->removeWidget( w );
			}
			return QWidget::event( ev );
		}
		default:
			return QWidget::event( ev );
	}
}

QSize DVHBox::sizeHint() const
{
	DVHBox* that = const_cast<DVHBox *>( this );
	QApplication::sendPostedEvents( that, QEvent::ChildAdded );
	return QFrame::sizeHint();
}

void DVHBox::setSpacing( int sp )
{
	layout()->setSpacing( sp );
}

void DVHBox::setStretchFactor( QWidget* w, int stretch )
{
	static_cast<QBoxLayout *>( layout() )->setStretchFactor( w, stretch );
}


void DVHBox::switchWidgetsPosition(QWidget *widget1, QWidget *widget2)
{
	int position1 = m_pLayout->indexOf(widget1);
	int position2 = m_pLayout->indexOf(widget2);
	
	if (position1 > 0 && position2 > 0 )
	{
	}
}

void DVHBox::mouseMoveEvent(QMouseEvent *e)
{
	if ( hasMouseTracking() )
	{
		emit mouseAt(e->pos());
	}
}

QBoxLayout *DVHBox::boxLayout()
{
	return m_pLayout;
}

