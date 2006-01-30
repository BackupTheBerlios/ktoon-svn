/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "acubictool.h"
#include "ktdebug.h"

ACubicTool::ACubicTool(): m_count(0), m_isComplete(false)
{
	m_path = QPainterPath();
}

ACubicTool::~ACubicTool()
{
	
}

QHash< QString, KTAction * > ACubicTool::actions()
{
	QHash<QString, KTAction *> hash;
	KTAction *cubic = new KTAction( QIcon(), tr("Cubic"), this);
// 	cubic->setShortcut( QKeySequence(tr("C")) );
	
	hash.insert( tr("Cubic"), cubic );
	
	return hash;
}

QPainterPath ACubicTool::path() const
{
	return m_path;
}

QRect ACubicTool::move(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& oldPos, const QPoint& newPos)
{
	m_nodes.last().right = newPos;
	m_nodes.last().left =  m_nodes.last().center - ( newPos - m_nodes.last().center)  ;
	

	QPainterPath path;
	QPainterPath ghost;

	if(m_nodes.count() > 1)
	{
		Node first = m_nodes[m_nodes.count()-2];
		Node second = m_nodes.last();
		path.moveTo(first.center);
		QPoint c = (first.center + second.center)/2;
		path.cubicTo( first.center, first.right , c);
		path.cubicTo( c,  second.left , second.center);
	}
	else
	{
// 		path.moveTo(m_nodes.first().center);
// 		ghost.moveTo(m_nodes.first().center);
	}
	
	
	ghost.moveTo(m_nodes.last().center);
	ghost.lineTo(m_nodes.last().left );
	ghost.moveTo(m_nodes.last().center);
	ghost.lineTo(m_nodes.last().right );
	m_path = path;
// 	m_path.addPath(path);
// 	ghost.addPath(drawNodes() );
	ghost.addPath(m_path);
	emit toDrawGhostGraphic( ghost );
	
	return m_path.boundingRect().toRect();
	
}

QRect ACubicTool::press(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos, KTKeyFrame* currentFrame)
{
	if(m_isComplete)
	{
		m_isComplete = false;
	}
	
	Node node;
	node.center = pos;
	m_nodes << node;
// 	ktDebug() << m_nodes.count();
	return move(brush, painter, form, pos, pos);
	
}

QRect ACubicTool::release(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos)
{
	painter.drawPath(m_path);
	return m_path.boundingRect().toRect();
}

QStringList ACubicTool::keys() const
{
	return QStringList() << "Cubic";
}


QPainterPath ACubicTool::drawNodes()
{
	QList< Node >::iterator it = m_nodes.end();
	QPainterPath path;
	if(m_nodes.count() > 1)
	{

		path.moveTo(m_nodes[0].center);
		for(int i = 0; i <  m_nodes.count()-1 ; i++)
		{
			QPoint c = (m_nodes[i].center + m_nodes[i+1].center)/2;
			path.cubicTo( m_nodes[i].center, m_nodes[i].right , c);
			path.cubicTo( c,  m_nodes[i+1].left , m_nodes[i+1].center);
		}
		
		
	}
// 	painter->drawPath(path);
	return path;
}

QWidget* ACubicTool::configurator()
{
	return 0;
}
int ACubicTool::type() const
{
	return Brush;
};

bool ACubicTool::isComplete() const
{
	return m_isComplete;
}

void ACubicTool::aboutToChangeTool() 
{
	m_isComplete = true;
	emit requestRedraw();
			
	m_nodes.clear();
}



Q_EXPORT_PLUGIN( ACubicTool );