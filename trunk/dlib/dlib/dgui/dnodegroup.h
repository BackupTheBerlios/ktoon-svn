
#ifndef NODEGROUP_H
#define NODEGROUP_H

#include "dgui/dcontrolnode.h"
#include <QGraphicsScene>

#include <QObject>
#include <QHash>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/


class DNodeGroup : public QObject
{
	Q_OBJECT
	public:
		DNodeGroup(QGraphicsItem * parent, QGraphicsScene *scene);
		~DNodeGroup();
		
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
		
		
	private:
		QList<DControlNode*> m_nodes;
		QGraphicsItem *m_parentItem;
		QPainterPath m_path;
		QPointF m_pos;
		QHash<int, QPointF > m_changedNodes;
		QGraphicsScene *m_scene;
		
};

#endif
