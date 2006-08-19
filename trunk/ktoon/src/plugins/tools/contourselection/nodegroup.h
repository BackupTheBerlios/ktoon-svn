
#ifndef NODEGROUP_H
#define NODEGROUP_H
#include "controlnode.h"
#include "ktscene.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class NodeGroup
{
	public:
		NodeGroup(QGraphicsItem * parent, KTScene *scene);
		~NodeGroup();
		
		QGraphicsItem * parentItem();
		
	private:
		QList<ControlNode*> m_nodes;
		QGraphicsItem *m_parentItem;
};

#endif
