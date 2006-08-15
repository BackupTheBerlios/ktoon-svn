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

NodeManager::NodeManager(QGraphicsItem * parent, KTScene *scene): m_parent(parent), m_moved(false)
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

void NodeManager::syncNodes(const QRectF sbr)
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
					(*it)->setPos( sbr.topLeft() );
					break;
				}
				case Node::TopRight:
				{
					(*it)->setPos(sbr.topRight());
					break;
				}
				case Node::BottomRight:
				{
					(*it)->setPos(sbr.bottomRight());
					break;
				}
				case Node::BottomLeft:
				{
					(*it)->setPos(sbr.bottomLeft());
					break;
				}
				case Node::Center:
				{
					(*it)->setPos(sbr.center());
					break;
				}
			}
			
			m_moved = true;
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

