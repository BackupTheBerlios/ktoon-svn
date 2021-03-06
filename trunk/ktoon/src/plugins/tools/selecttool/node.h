/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
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
		enum ActionNode{Scale = 0, Rotate};
		
		Node(TypeNode node, ActionNode action, const QPointF & pos = QPoint(0,0) , NodeManager *manager = 0, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0 );
		~Node();
		
		
		
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
		QRectF boundingRect() const;
		
		
		void setAction(ActionNode action);
		int actionNode();
		
		enum { Type = UserType + 1 };
		int type() const { return Type; }
		
		int typeNode() const;
		
	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
		void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
		
	private:
		struct Private;
		Private *const k;
		
};

#endif
