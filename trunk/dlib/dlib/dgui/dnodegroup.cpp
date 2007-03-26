#include "dnodegroup.h"

#include <dcore/ddebug.h>

#include <QGraphicsPathItem>
#include <QAbstractGraphicsShapeItem>


DNodeGroup::DNodeGroup(QGraphicsItem * parent, QGraphicsScene *scene): m_parentItem(parent), m_scene(scene)
{
	DINIT;
	QGraphicsPathItem *tmp = qgraphicsitem_cast<QGraphicsPathItem *>(parent);
	if(tmp)
	{
		QPainterPath path = tmp->sceneMatrix().map( tmp->path());
		saveParentProperties();
		int index = 0;
		
		while(index < path.elementCount())
		{
			QPainterPath::Element e = path.elementAt(index);
			
			if(e.type == QPainterPath::CurveToDataElement)
			{
				if(index - 2 < 0) continue;
				if( path.elementAt(index-2).type == QPainterPath::CurveToElement )
				{
					DControlNode *node = new DControlNode(index, this,  path.elementAt(index), tmp, scene);
					QPainterPath::Element e1 = path.elementAt(index-1);
					node->setLeft(new DControlNode(index-1,this, e1, tmp, scene));
					
					if(index+1 < path.elementCount() )
					{
						QPainterPath::Element e2 = path.elementAt(index+1);
						if(e2.type == QPainterPath::CurveToElement)
						{
							node->setRight(new DControlNode(index+1, this, e2, tmp, scene));
							m_nodes << node->right();
							index++;
						}
					}
					m_nodes << node;
					m_nodes << node->left();
				}
			}
			else if( (e.type == QPainterPath::LineToElement || e.type == QPainterPath::MoveToElement ) )
			{
				if(index+1 < path.elementCount())
				{
					if( path.elementAt(index+1).type == QPainterPath::CurveToElement )
					{
						DControlNode *node = new DControlNode(index, this, path.elementAt(index), tmp, scene);
						node->setRight(new DControlNode(index+1,this, path.elementAt(index+1), tmp, scene));
						
						index++;
						m_nodes << node;
						m_nodes << node->right();
					}
				}
			}
			index++;
		}
	}
	else
	{
		dDebug("selection") << "Item not item path";
	}
	
}



QGraphicsItem * DNodeGroup::parentItem()
{
	return m_parentItem;
}

DNodeGroup::~DNodeGroup()
{
	DEND;
	qDeleteAll(m_nodes);
	m_nodes.clear();
	
}


void DNodeGroup::syncNodes(const QPainterPath & path)
{
	if(m_nodes.isEmpty())
	{
		return;
	}
	foreach(DControlNode *node, m_nodes)
	{
		node->setNotChange(true);
		node->setPos(path.elementAt(node->index()));
	}
}

void DNodeGroup::syncNodesFromParent()
{
	if(m_parentItem)
	{
		if(qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem))
		{
			syncNodes(m_parentItem->sceneMatrix().map( qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path()));
		}
	}
}

void DNodeGroup::setParentItem(QGraphicsItem *newParent)
{
	D_FUNCINFO;
	m_parentItem = newParent;
	foreach(DControlNode *node, m_nodes)
	{
		if(node)
		{
			node->setParentI(newParent);
		}
	}
}

void DNodeGroup::moveElementTo(int index, const QPointF& pos )
{

	QPainterPath path = qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path();
	path.setElementPositionAt(index,pos.x(), pos.y() );
	QPainterPath::Element e = path.elementAt(0);
	qgraphicsitem_cast<QGraphicsPathItem *>( m_parentItem)->setPath(path);
	
	if(m_changedNodes.contains (index))
	{
		(*m_changedNodes.find(index)) = pos;
	}
	else
	{
		m_changedNodes.insert(index, pos);
	}
	
}

QHash<int, QPointF > DNodeGroup::changedNodes()
{
	return m_changedNodes;
}

void DNodeGroup::clearChangesNodes()
{
	m_changedNodes.clear();
}

void DNodeGroup::restoreItem()
{
	qgraphicsitem_cast<QGraphicsPathItem *>( m_parentItem)->setPath(m_path);
	m_parentItem->setPos(m_pos);
}

void DNodeGroup::show()
{
	foreach(DControlNode *node, m_nodes)
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

void DNodeGroup::saveParentProperties()
{
	if(qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem))
	{
		m_path = qgraphicsitem_cast<QGraphicsPathItem *>(m_parentItem)->path();
		m_pos = m_parentItem->scenePos();
	}
}

int DNodeGroup::removeSelectedNodes()
{
	int count = 0;
	foreach(DControlNode *node, m_nodes )
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

void DNodeGroup::addControlNode(DControlNode* node)
{
	
}

