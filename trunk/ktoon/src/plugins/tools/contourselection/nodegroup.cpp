#include "nodegroup.h"

#include <ddebug.h>

#include <QGraphicsPathItem>
#include <QAbstractGraphicsShapeItem>


NodeGroup::NodeGroup(QGraphicsItem * parent, KTScene *scene): m_parentItem(parent)
{
	QAbstractGraphicsShapeItem *tmp = dynamic_cast<QAbstractGraphicsShapeItem *>(parent);

	if(tmp)
	{
// 		QPainterPath path = tmp->sceneMatrix().map( tmp->path());
		QPainterPath path = tmp->sceneMatrix().map( tmp->shape());
		
		int index = 0;
		while(index < path.elementCount())
		{
			QPainterPath::Element e = path.elementAt(index);
			if(e.type == QPainterPath::CurveToDataElement)
			{
				if(index - 2 < 0) continue;
				if( path.elementAt(index-2).type == QPainterPath::CurveToElement )
				{
					ControlNode *node = new ControlNode(index, this,  path.elementAt(index), tmp, scene);
					QPainterPath::Element e1 = path.elementAt(index-1);
					QPainterPath::Element e2 = path.elementAt(index+1);
					node->setLeft(new ControlNode(index-1,this, e1, tmp, scene));
					if(index+1 < path.elementCount() && e2.type == QPainterPath::CurveToElement)
					{
						node->setRight(new ControlNode(index+1, this, e2, tmp, scene));
						m_nodes << node->right();
						index++;
					}
					m_nodes << node;
					m_nodes << node->left();
				}
			}
			else if( (e.type == QPainterPath::LineToElement || e.type == QPainterPath::MoveToElement ) &&  path.elementAt(index+1).type == QPainterPath::CurveToElement )
			{
				ControlNode *node = new ControlNode(index, this, path.elementAt(index), tmp, scene);
				node->setRight(new ControlNode(index+1,this, path.elementAt(index+1), tmp, scene));
				index++;
				m_nodes << node;
				m_nodes << node->right();
			}
			else
			{
				ControlNode *node = new ControlNode(index, this, path.elementAt(index), tmp, scene);
				m_nodes << node;
			}
			index++;
		}
	}
}



QGraphicsItem * NodeGroup::parentItem()
{
	return m_parentItem;
}

NodeGroup::~NodeGroup()
{
	qDeleteAll(m_nodes);
	m_nodes.clear();
}


void NodeGroup::syncNodes(const QPainterPath & path)
{
	
	if(m_nodes.isEmpty())
	{
		return;
	}
	foreach(ControlNode *node, m_nodes)
	{
		node->setPos(path.elementAt(node->index()));
	}

}

void NodeGroup::syncNodesFromParent()
{
	if(m_parentItem)
	{
// 		syncNodes(m_parentItem->sceneMatrix().map( m_parentItem->shape()));
	}
}

void NodeGroup::setParentItem(QGraphicsItem *newParent)
{
	D_FUNCINFO;
	m_parentItem = newParent;
	foreach(ControlNode *node, m_nodes)
	{
		node->setParentI(newParent);
	}
}


