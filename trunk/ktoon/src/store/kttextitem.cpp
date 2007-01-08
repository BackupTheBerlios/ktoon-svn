/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "kttextitem.h"
#include "ktserializer.h"

#include <QFont>
#include <QFocusEvent>
#include <QTimer>

KTTextItem::KTTextItem(QGraphicsItem * parent, QGraphicsScene * scene)
	: QGraphicsTextItem(parent, scene), m_flags(flags()), m_isEditable(false)
{
	setOpenExternalLinks(true);
	setEditable( false );
}


KTTextItem::~KTTextItem()
{
}

void KTTextItem::fromXml(const QString &xml)
{
}


QDomElement KTTextItem::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("text");
	
	QDomText text = doc.createTextNode( toHtml() );
	root.appendChild( text );
	
	root.appendChild( KTSerializer::properties( this, doc));
	QFont font = this->font();
	
	root.appendChild( KTSerializer::font( &font, doc ) );
	
	return root;
}

void KTTextItem::setEditable(bool editable)
{
	m_isEditable = editable;
	
	if ( editable )
	{
		m_flags = flags(); // save flags
		setTextInteractionFlags(Qt::TextEditorInteraction);
		setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
		setFocus(Qt::MouseFocusReason);
	}
	else
	{
		setTextInteractionFlags(Qt::TextBrowserInteraction);
		setFlags(  QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable ); // restore flags
	}
	update();
}

void KTTextItem::toggleEditable()
{
	setEditable( !m_isEditable );
}

void KTTextItem::focusOutEvent(QFocusEvent * event )
{
	QGraphicsTextItem::focusOutEvent(event);
	if ( textInteractionFlags() & Qt::TextEditorInteraction && m_isEditable )
	{
		QTimer::singleShot( 0, this, SLOT(toggleEditable()));
		emit edited();
	}
}

void KTTextItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
	setEditable( true );
}

