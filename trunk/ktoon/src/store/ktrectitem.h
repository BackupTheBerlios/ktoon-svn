/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTRECTITEM_H
#define KTRECTITEM_H

#include <QGraphicsRectItem>

#include "ktabstractserializable.h"
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTRectItem : public QGraphicsRectItem, public KTAbstractSerializable
{
	public:
		KTRectItem( QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		KTRectItem( const QRectF& rect, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		~KTRectItem();
		bool contains ( const QPointF & point ) const;
		
		virtual void fromXml(const QString &xml);
		virtual QDomElement toXml(QDomDocument &doc) const;
		
	protected:
		virtual void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
		virtual void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
		virtual void dropEvent ( QGraphicsSceneDragDropEvent *event );
		
	private:
		bool m_dragOver;
};

#endif

