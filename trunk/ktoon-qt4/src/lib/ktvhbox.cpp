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

#include "ktvhbox.h"
#include "ktdebug.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPoint>

KTVHBox::KTVHBox(QWidget *parent, Qt::Orientation o) : QFrame(parent)
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

KTVHBox::KTVHBox(QWidget *parent, bool isVertical) : QFrame(parent)
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


KTVHBox::~KTVHBox()
{
}

void KTVHBox::addWidget(QWidget *child, Qt::Alignment alignment)
{
	child->setParent(this);
	m_pLayout->addWidget(child);
	if ( alignment != 0 )
	{
		m_pLayout->setAlignment(child, alignment);
	}
}

void KTVHBox::moveWidgetUp(QWidget *widget)
{
	ktDebug() << "Childs " << children ().count() << endl;
	int position = m_pLayout->findWidget(widget);
	
	
	ktDebug() << "Position: " << position << endl;
	
	if (position > 0 )
	{
		m_pLayout->remove(widget);
		m_pLayout->insertWidget(position-1, widget);
	}
	else
	{
		ktDebug() << "The widget isn't in the layout" << endl;
	}
}

void KTVHBox::moveWidgetDown(QWidget *widget)
{
	int position = m_pLayout->findWidget(widget);
	
	if (position >= 0 )
	{
		m_pLayout->remove(widget);
		m_pLayout->insertWidget(position+1, widget);
	}
}

bool KTVHBox::event( QEvent* ev )
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

QSize KTVHBox::sizeHint() const
{
	KTVHBox* that = const_cast<KTVHBox *>( this );
	QApplication::sendPostedEvents( that, QEvent::ChildAdded );
	return QFrame::sizeHint();
}

void KTVHBox::setSpacing( int sp )
{
	layout()->setSpacing( sp );
}

void KTVHBox::setStretchFactor( QWidget* w, int stretch )
{
	static_cast<QBoxLayout *>( layout() )->setStretchFactor( w, stretch );
}


void KTVHBox::switchWidgetsPosition(QWidget *widget1, QWidget *widget2)
{
	int position1 = m_pLayout->findWidget(widget1);
	int position2 = m_pLayout->findWidget(widget2);
	
	if (position1 > 0 && position2 > 0 )
	{
	}
}

void KTVHBox::mouseMoveEvent(QMouseEvent *e)
{
	if ( hasMouseTracking() )
	{
		emit mouseAt(e->pos());
	}
}

QBoxLayout *KTVHBox::boxLayout()
{
	return m_pLayout;
}

