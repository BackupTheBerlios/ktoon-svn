
#ifndef NODEGROUP_H
#define NODEGROUP_H
#include "controlnode.h"
#include "ktscene.h"

#include <QObject>

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
		
		void emitEvent(KTItemEvent *event);
		
	private:
		QList<ControlNode*> m_nodes;
		QGraphicsItem *m_parentItem;
		
	signals:
		void sendEvent( KTProjectEvent *event );
};

#endif
