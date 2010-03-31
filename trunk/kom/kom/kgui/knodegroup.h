/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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
 *                                                                         *
 *   License:                                                              *
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


#ifndef NODEGROUP_H
#define NODEGROUP_H

#include "kgui/kcontrolnode.h"
#include <QGraphicsScene>

#include <QObject>
#include <QHash>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/


class KNodeGroup : public QObject
{
	Q_OBJECT
	public:
		KNodeGroup(QGraphicsItem * parent, QGraphicsScene *scene);
		~KNodeGroup();
		
		QGraphicsItem * parentItem();
		void syncNodes(const QPainterPath & path);
		void syncNodesFromParent();
		
		void setParentItem(QGraphicsItem *);
		
		void moveElementTo(int index, const QPointF& pos );
		QHash<int, QPointF > changedNodes();
		void clearChangesNodes();
		void restoreItem();
		void show();
		void saveParentProperties();
		
		int removeSelectedNodes();
		
		void addControlNode(KControlNode* node);
		
		void createNodes(QGraphicsPathItem *nodes);
		void emitNodeClicked();
		
		void expandAllNodes();
		bool isSelected();
		
	private:
		struct Private;
		Private *const k;
		
	signals:
		void itemChanged(QGraphicsItem *item );
		void nodeClicked();
		
};

#endif
