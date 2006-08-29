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
#include "ktscene.h"


/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class NodeGroup;
class ControlNode : public QObject, public QGraphicsItem
{
	Q_OBJECT;
	
	public:
		
		ControlNode(int index, NodeGroup *nodeGroup, const QPointF & pos = QPoint(0,0) ,  QGraphicsItem * parent = 0, KTScene * scene = 0 );
		
		~ControlNode();
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
		QRectF boundingRect() const;
		enum { Type = UserType + 1 };
		
		int type() const { return Type; }
// 		int typeNodeEditor() const;
		
		void setLeft( ControlNode *left);
		void setRight( ControlNode *right);
		void setNodeParent( ControlNode *nodeParent);
		int index() const;
		
		void setParentI( QGraphicsItem * newParent );
		QGraphicsItem *parentI();
		
		
		ControlNode *left();
		ControlNode *right();
		ControlNode *nodeParent();
		
	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
// 		virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event );
	private:
		void paintLinesToChilds(QPainter * painter);
	
	public slots:
		void  repaint();
		void setVisibleChilds(bool visible);
		
	signals:
		void requestUpdateParent();
		void showBrothers(bool show);
		
	private:
		int m_index;
		QGraphicsItem * m_parent;
		
		ControlNode *m_left, *m_right, *m_nodeParent;
		bool m_notChange;
		NodeGroup *m_nodeGroup;
		KTScene *m_scene;
};

#endif
