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

#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>
#include <QGraphicsScene>


/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class DNodeGroup;
class DControlNode : public QObject, public QGraphicsItem
{
	Q_OBJECT;
	
	public:
		
		DControlNode(int index, DNodeGroup *nodeGroup, const QPointF & pos = QPoint(0,0) ,  QGraphicsItem * parent = 0, QGraphicsScene * scene = 0 );
		
		~DControlNode();
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
		QRectF boundingRect() const;
		enum { Type = UserType + 100 };
		
		int type() const { return Type; }
// 		int typeNodeEditor() const;
		
		void setLeft( DControlNode *left);
		void setRight( DControlNode *right);
		void setNodeParent( DControlNode *nodeParent);
		int index() const;
		
		void setParentI( QGraphicsItem * newParent ); // ### setParentI no es una convencion, buscar un mejor nombre
		QGraphicsItem *parentI();
		
		
		DControlNode *left();
		DControlNode *right();
		DControlNode *nodeParent();
		
		void setNotChange(bool notChange);
		
	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
		
	private:
		void paintLinesToChilds(QPainter * painter);
		
	public slots:
// 		void repaint();
		void setVisibleChilds(bool visible);
		void setSeletedChilds(bool select);
		
	signals:
// 		void requestUpdateParent();
		void showBrothers(bool show);
		
	private:
		int m_index;
		QGraphicsItem * m_parent;
		
		DControlNode *m_left, *m_right, *m_nodeParent;
		bool m_notChange;
		DNodeGroup *m_nodeGroup;
		QGraphicsScene *m_scene;
};

#endif
