#include "dnodegroup.h"

#include <dcore/ddebug.h>

#include <QGraphicsPathItem>
#include <QAbstractGraphicsShapeItem>

struct DNodeGroup::Private
{
	QList<DControlNode*> nodes;
	QGraphicsItem *parentItem;
	QPainterPath path;
	QPointF pos;
	QHash<int, QPointF > changedNodes;
	QGraphicsScene *scene;
};


DNodeGroup::DNodeGroup(QGraphicsItem * parent, QGraphicsScene *scene): d(new Private)
{
	DINIT;
	
	d->parentItem = parent;
	d->scene = scene;
	
	if(QGraphicsPathItem *pathItem = qgraphicsitem_cast<QGraphicsPathItem *>(parent))
	{
		createNodes(pathItem);
	}
}


QGraphicsItem * DNodeGroup::parentItem()
{
	return d->parentItem;
}


DNodeGroup::~DNodeGroup()
{
	DEND;
	qDeleteAll(d->nodes);
	d->nodes.clear();
	delete d;
}


void DNodeGroup::syncNodes(const QPainterPath & path)
{
	if(d->nodes.isEmpty())
	{
		return;
	}
	foreach(DControlNode *node, d->nodes)
	{
		node->setNotChange(true);
		node->setPos(path.elementAt(node->index()));
	}
}


void DNodeGroup::syncNodesFromParent()
{
	if(d->parentItem)
	{
		if(qgraphicsitem_cast<QGraphicsPathItem *>(d->parentItem))
		{
			syncNodes(d->parentItem->sceneMatrix().map( qgraphicsitem_cast<QGraphicsPathItem *>(d->parentItem)->path()));
		}
	}
}


void DNodeGroup::setParentItem(QGraphicsItem *newParent)
{
	D_FUNCINFO;
	d->parentItem = newParent;
	foreach(DControlNode *node, d->nodes)
	{
		if(node)
		{
			node->setParentI(newParent);
		}
	}
}


void DNodeGroup::moveElementTo(int index, const QPointF& pos )
{

	QPainterPath path = qgraphicsitem_cast<QGraphicsPathItem *>(d->parentItem)->path();
	path.setElementPositionAt(index,pos.x(), pos.y() );
	QPainterPath::Element e = path.elementAt(0);
	qgraphicsitem_cast<QGraphicsPathItem *>( d->parentItem)->setPath(path);
	
	if(d->changedNodes.contains (index))
	{
		(*d->changedNodes.find(index)) = pos;
	}
	else
	{
		d->changedNodes.insert(index, pos);
		emit itemChanged( d->parentItem );
	}
}


QHash<int, QPointF > DNodeGroup::changedNodes()
{
	return d->changedNodes;
}


void DNodeGroup::clearChangesNodes()
{
	d->changedNodes.clear();
}


void DNodeGroup::restoreItem()
{
	qgraphicsitem_cast<QGraphicsPathItem *>( d->parentItem)->setPath(d->path);
	d->parentItem->setPos(d->pos);
}


void DNodeGroup::show()
{
	foreach(DControlNode *node, d->nodes)
	{
		if(qgraphicsitem_cast<QGraphicsPathItem *>( d->parentItem))
		{
			if(!node->scene())
			{
				d->scene->addItem(node);
			}
		}
	}
}


void DNodeGroup::saveParentProperties()
{
	if(qgraphicsitem_cast<QGraphicsPathItem *>(d->parentItem))
	{
		d->path = qgraphicsitem_cast<QGraphicsPathItem *>(d->parentItem)->path();
		d->pos = d->parentItem->scenePos();
	}
}


int DNodeGroup::removeSelectedNodes()
{
	int count = 0;
	foreach(DControlNode *node, d->nodes )
	{
		if ( node->isSelected() )
		{
			count++;
			
			d->nodes.removeAll(node);
			// FIXME: re-crear el path.
		}
	}
	
	return count;
}


void DNodeGroup::createNodes(QGraphicsPathItem *pathItem)
{
	if(pathItem)
	{
		foreach(DControlNode *node, d->nodes)
		{
			d->scene->removeItem(node);
		}
		qDeleteAll(d->nodes);
		d->nodes.clear();
		
		QPainterPath path = pathItem->sceneMatrix().map( pathItem->path());
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
					DControlNode *node = new DControlNode(index, this,  path.elementAt(index), pathItem, d->scene);
					QPainterPath::Element e1 = path.elementAt(index-1);
					node->setLeft(new DControlNode(index-1,this, e1, pathItem, d->scene));
					
					if(index+1 < path.elementCount() )
					{
						QPainterPath::Element e2 = path.elementAt(index+1);
						if(e2.type == QPainterPath::CurveToElement)
						{
							node->setRight(new DControlNode(index+1, this, e2, pathItem, d->scene));
							d->nodes << node->right();
							index++;
						}
					}
					d->nodes << node;
					d->nodes << node->left();
				}
			}
			else if( (e.type == QPainterPath::LineToElement || e.type == QPainterPath::MoveToElement ) )
			{
				DControlNode *node;
				if(index+1 < path.elementCount())
				{
					
					if( path.elementAt(index+1).type == QPainterPath::CurveToElement )
					{
						node = new DControlNode(index, this, path.elementAt(index), pathItem, d->scene);
						node->setRight(new DControlNode(index+1,this, path.elementAt(index+1), pathItem, d->scene));
						
						index++;
						d->nodes << node;
						d->nodes << node->right();
					}
					else
					{
						node = new DControlNode(index, this, path.elementAt(index), pathItem, d->scene);
						d->nodes << node;
					}
				}
				else
				{
					node = new DControlNode(index, this, path.elementAt(index), pathItem, d->scene);
					d->nodes << node;
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

void DNodeGroup::addControlNode(DControlNode* )
{
	
}

void DNodeGroup::emitNodeClicked()
{
	emit nodeClicked();
}

