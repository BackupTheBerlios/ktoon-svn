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

#include "ktbuttonitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>

KTButtonItem::KTButtonItem(QGraphicsItem * parent, QGraphicsScene * scene)
	: QGraphicsItem(parent, scene)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
	setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable  );
	
	
	m_iconSize = QSize(22,22);
}


KTButtonItem::~KTButtonItem()
{
}


void KTButtonItem::fromXml(const QString &xml)
{
}


QDomElement KTButtonItem::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("button");
	
	return root;
}


void KTButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	QStyleOptionButton optionButton;
	
	optionButton.text = m_text;
	optionButton.rect = boundingRect().toRect();
	optionButton.icon = m_icon;
	optionButton.iconSize = m_iconSize;
	
	if (option->state & QStyle::State_Sunken)
	{
		optionButton.state = option->state;
	}
	else
	{
		
	}
	QApplication::style()->drawControl( QStyle::CE_PushButton, &optionButton ,painter, w );
}

QRectF KTButtonItem::boundingRect() const
{
	double width = 2;
	double height = 2;
	
	QFontMetricsF fm(m_font);
	
	width += fm.width( m_text );
	height += fm.height();
	
	width += m_iconSize.width();
	height += m_iconSize.height();
	
	
	
	return QRectF(0, 0, width, height);
}

void KTButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
	update();
}

void KTButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	update();
}

void KTButtonItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseMoveEvent(event);
	
	emit clicked();
	
	update();
}

void KTButtonItem::setIconSize(const QSize &size)
{
	m_iconSize = size;
	update();
}

void KTButtonItem::setIcon(const QIcon &icon)
{
	m_icon = icon;
	update();
}

void KTButtonItem::setText(const QString &text)
{
	m_text = text;
	update();
}

void KTButtonItem::setFont(const QFont &font)
{
	m_font = font;
	update();
}

