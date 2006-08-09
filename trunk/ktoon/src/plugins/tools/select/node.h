/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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

#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>



/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class NodeManager;
class Node : public QObject, public QGraphicsItem
{
	Q_OBJECT;
	
	public:
		
		enum TypeNode{TopLeft  = 0, TopRight, BottomLeft, BottomRight, Center   };
		Node(TypeNode node, const QPointF & pos = QPoint(0,0) , NodeManager *manager = 0, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0 );
		~Node();
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
		QRectF boundingRect() const;
		enum { Type = UserType + 1 };
		
		int type() const { return Type; }
		int typeNode() const;
		
	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
		
	private:
		TypeNode m_typeNode;
		bool m_notChange;
		QRectF m_brParent;
		QGraphicsItem * m_parent;
		NodeManager *m_manager;
		
		QGraphicsRectItem *gb1, *gb2 ;
		
		
};

#endif
