/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "nodemanager.h"
#include <ddebug.h>
NodeManager::NodeManager(QGraphicsItem * parent, KTScene *scene): m_parent(parent), m_scene(scene), m_modify(false), m_anchor(0,0), m_press(false)
{
	QRectF rect = parent->sceneBoundingRect();
	Node *topLeft = new Node(Node::TopLeft, rect.topLeft(), this, parent, scene);
	Node *topRight = new Node(Node::TopRight, rect.topRight(), this, parent, scene);
	Node *bottomLeft = new Node(Node::BottomLeft, rect.bottomLeft(),this, parent, scene);
	Node *bottomRight = new Node(Node::BottomRight, rect.bottomRight(),this, parent, scene);
	Node *center = new Node(Node::Center, rect.center() ,this,parent, scene);
	m_nodes.insert(Node::TopLeft, topLeft);
	m_nodes.insert(Node::TopRight, topRight);
	m_nodes.insert(Node::BottomLeft, bottomLeft );
	m_nodes.insert(Node::BottomRight, bottomRight );
	m_nodes.insert(Node::Center, center );
	
	beginToEdit();
}



NodeManager::~NodeManager()
{
	foreach(Node *node, m_nodes)
	{
		if(node)
		{
			if ( QGraphicsScene *scene = node->scene() )
			{
				scene->removeItem(node);
			}
		}
		delete node;
		node = 0;
	}
	m_nodes.clear();
	
}

void NodeManager::syncNodes(const QRectF &sbr)
{
	if(m_nodes.isEmpty())
	{
		return;
	}
	
	QHash<Node::TypeNode, Node *>::iterator it = m_nodes.begin();
	while(it != m_nodes.end())
	{
		if((*it))
		{
			switch(it.key())
			{
				case Node::TopLeft:
				{
					if((*it)->scenePos() != sbr.topLeft() )
					{
						(*it)->setPos( sbr.topLeft() );
						m_modify = true;
					}
					break;
				}
				case Node::TopRight:
				{
					if((*it)->scenePos() != sbr.topRight() )
					{
						(*it)->setPos( sbr.topRight() );
						m_modify = true;
					}
					break;
				}
				case Node::BottomRight:
				{
					if((*it)->scenePos() != sbr.bottomRight() )
					{
						(*it)->setPos( sbr.bottomRight() );
						m_modify = true;
					}
					break;
				}
				case Node::BottomLeft:
				{
					if((*it)->scenePos() != sbr.bottomLeft() )
					{
						(*it)->setPos( sbr.bottomLeft() );
						m_modify = true;
					}
					break;
				}
				case Node::Center:
				{
					if((*it)->scenePos() != sbr.center() )
					{
						(*it)->setPos( sbr.center() );
						m_modify = true;
					}
					break;
				}
			}
		}
		++it;
	}
}

void NodeManager::syncNodesFromParent()
{
	if(m_parent)
	{
		syncNodes(m_parent->sceneBoundingRect());
	}
}

QGraphicsItem *NodeManager::parentItem() const
{
	return m_parent;
}

bool NodeManager::isModified() const
{
	return m_modify;
}

void NodeManager::setModify(bool modify)
{
	m_modify = modify;
}

void NodeManager::beginToEdit()
{
	m_origMatrix = m_parent->matrix();
	m_origPos = m_parent->pos();
}

void NodeManager::restoreItem()
{
	m_parent->setMatrix(m_origMatrix);
	m_parent->setPos(m_origPos);
}


void NodeManager::setAnchor(const QPointF& point)
{
	m_anchor = point;
}

void NodeManager::scale(float sx, float sy)
{
	QMatrix m = m_parent->matrix();
	m.translate(m_anchor.x(),m_anchor.y());
	m.scale(sx,sy);
	m.translate(-m_anchor.x(),-m_anchor.y());
	m_parent->setMatrix(m);
	syncNodesFromParent();
}

void NodeManager::show()
{
	foreach(Node *node, m_nodes)
	{
		if(!node->scene())
		{
			m_scene->addItem(node);
		}
	}
}

void NodeManager::setPress(bool press)
{
	m_press = press;
}

bool NodeManager::isPress()
{
	return m_press;
}
