#include "nodegroup.h"
#include <QGraphicsPathItem>

NodeGroup::NodeGroup(QGraphicsItem * parent, KTScene *scene): m_parentItem(parent)
{
	QGraphicsPathItem *tmp = dynamic_cast<QGraphicsPathItem *>(parent);
	if(tmp)
	{
		QPainterPath path = tmp->matrix().map( tmp->path());
		QMatrix m;
		QPointF t = tmp->pos();
		m.translate( t.x(), t.y() );
		path = m.map( path );
		int index = 0;
		while(index < path.elementCount())
		{
			QPainterPath::Element e = path.elementAt(index);
			if(e.type == QPainterPath::CurveToDataElement)
			{
				if(index - 2 < 0) continue;
							
				if( path.elementAt(index-2).type == QPainterPath::CurveToElement )
				{
					ControlNode *node = new ControlNode(index, path.elementAt(index), tmp, scene);
					QPainterPath::Element e1 = path.elementAt(index-1);
					QPainterPath::Element e2 = path.elementAt(index+1);
					node->setLeft(new ControlNode(index-1, e1, tmp, scene));
					if(index+1 < path.elementCount() && e2.type == QPainterPath::CurveToElement)
					{
						node->setRight(new ControlNode(index+1, e2, tmp, scene));
						m_nodes << node->right();
						index++;
					}
									
					m_nodes << node;
					m_nodes << node->left();
				}
			}
			else if( (e.type == QPainterPath::LineToElement || e.type == QPainterPath::MoveToElement ) &&  path.elementAt(index+1).type == QPainterPath::CurveToElement )
			{
				ControlNode *node = new ControlNode(index,path.elementAt(index), tmp, scene);
								
				node->setRight(new ControlNode(index+1, path.elementAt(index+1), tmp, scene));
								
				index++;
								
				m_nodes << node;
				m_nodes << node->right();
			}
			else
			{
				ControlNode *node = new ControlNode(index, path.elementAt(index), tmp, scene);
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


