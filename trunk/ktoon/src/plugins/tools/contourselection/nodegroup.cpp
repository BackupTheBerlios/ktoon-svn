#include "nodegroup.h"

#include <ddebug.h>

#include <QGraphicsPathItem>
#include <QAbstractGraphicsShapeItem>


NodeGroup::NodeGroup(QGraphicsItem * parent, KTScene *scene): m_parentItem(parent), m_scene(scene)
{
	DINIT;
	QGraphicsPathItem *tmp = qgraphicsitem_cast<QGraphicsPathItem *>(parent);
	if(tmp)
	{
		QPainterPath path = tmp->sceneMatrix().map( tmp->path());
		saveParentProperties();
		int index = 0;
		
		while(index < path.elementCount()-1)
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
		
		ControlNode *node = new ControlNode(path.elementCount()-1, this, path.elementAt(path.elementCount()-1), tmp, scene);
		m_nodes << node;
		
	}
	else
	{
		dDebug("selection") << "Item not item path";
	}
	
}



QGraphicsItem * NodeGroup::parentItem()
{
	return m_parentItem;
}

NodeGroup::~NodeGroup()
{
	DEND;
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
		node->setNotChange(true);
		node->setPos(path.elementAt(node->index()));
	}
}

void NodeGroup::syncNodesFromParent()
{
	if(m_parentItem)
	{
		if(qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem))
		{
			syncNodes(m_parentItem->sceneMatrix().map( qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path()));
		}
	}
}

void NodeGroup::setParentItem(QGraphicsItem *newParent)
{
	D_FUNCINFO;
	m_parentItem = newParent;
	foreach(ControlNode *node, m_nodes)
	{
		if(node)
		{
			node->setParentI(newParent);
		}
	}
}

void NodeGroup::moveElementTo(int index, const QPointF& pos )
{

	QPainterPath path = qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path();
	path.setElementPositionAt(index,pos.x(), pos.y() );
	QPainterPath::Element e = path.elementAt(0);
	qgraphicsitem_cast<QGraphicsPathItem *>( m_parentItem)->setPath(path);
	
	if(m_changedsNodes.contains (index))
	{
		(*m_changedsNodes.find(index)) = pos;
	}
	else
	{
		m_changedsNodes.insert(index, pos);
	}
	
}

QHash<int, QPointF > NodeGroup::changedsNodes()
{
	return m_changedsNodes;
}

void NodeGroup::clearChangesNodes()
{
	m_changedsNodes.clear();
}

void NodeGroup::restoreItem()
{
	qgraphicsitem_cast<QGraphicsPathItem *>( m_parentItem)->setPath(m_path);
	m_parentItem->setPos(m_pos);
}

void NodeGroup::show()
{
	foreach(ControlNode *node, m_nodes)
	{
		if(qgraphicsitem_cast<QGraphicsPathItem *>( m_parentItem))
		{
			if(!node->scene())
			{
				m_scene->addItem(node);
			}
		}
	}
}

void NodeGroup::saveParentProperties()
{
	if(qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem))
	{
		m_path = qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path();
		m_pos = m_parentItem->scenePos();
	}
}

int NodeGroup::removeSelectedNodes()
{
	int count = 0;
	foreach(ControlNode *node, m_nodes )
	{
		if ( node->isSelected() )
		{
			count++;
			
			m_nodes.removeAll(node);
			// FIXME: re-crear el path.
		}
	}
	
	return count;
}


