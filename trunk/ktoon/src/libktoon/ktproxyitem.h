/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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

#ifndef KTPROXYITEM_H
#define KTPROXYITEM_H

#include <QGraphicsItem>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTProxyItem : public QGraphicsItem
{
	public:
		KTProxyItem(QGraphicsItem *item = 0);
		~KTProxyItem();
		
		void setItem(QGraphicsItem *item);
		QGraphicsItem *item() const;
		
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
		
		virtual bool collidesWithItem ( const QGraphicsItem * other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape ) const;
		virtual bool collidesWithPath ( const QPainterPath & path, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape ) const;
		virtual bool contains ( const QPointF & point ) const;
		virtual bool isObscuredBy ( const QGraphicsItem * item ) const;
		virtual QPainterPath opaqueArea () const;
		
		
		QPainterPath shape () const;
		
		enum { Type = UserType + 2 };
		int type() const { return Type; }
		
	private:
		struct Private;
		Private *const d;
};

#endif


