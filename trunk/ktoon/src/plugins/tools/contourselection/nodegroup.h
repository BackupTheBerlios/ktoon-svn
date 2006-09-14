
#ifndef NODEGROUP_H
#define NODEGROUP_H
#include "controlnode.h"
#include "ktscene.h"

#include <QObject>
#include <QHash>
#include "ktitemevent.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/


class NodeGroup : public QObject
{
	Q_OBJECT
	public:
		NodeGroup(QGraphicsItem * parent, KTScene *scene);
		~NodeGroup();
		
		QGraphicsItem * parentItem();
		void NodeGroup::syncNodes(const QPainterPath & path);
		void NodeGroup::syncNodesFromParent();
		
		void setParentItem(QGraphicsItem *);
		void moveElemetTo(int index, const QPointF& pos );
		QHash<int, QPointF > changedsNodes();
		void clearChangesNodes();
		void restoreItem();
		void show();
		void saveParentProperties();
		
	private:
		QList<ControlNode*> m_nodes;
		QGraphicsItem *m_parentItem;
		QPainterPath m_path;
		QPointF m_pos;
		QHash<int, QPointF > m_changedsNodes;
		KTScene *m_scene;
		
};

#endif
