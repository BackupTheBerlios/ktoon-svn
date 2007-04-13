
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
		
		void addControlNode(DControlNode* node);
		
		void createNodes(QGraphicsPathItem *nodes);
		void emitNodeClicked();
		
		void expandAllNodes();
		bool isSelected();
		
	private:
		struct Private;
		Private *const d;
		
	signals:
		void itemChanged(QGraphicsItem *item );
		void nodeClicked();
		
};

#endif
