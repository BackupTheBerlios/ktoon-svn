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

#include "apolygontool.h"
#include "dglobal.h"

#include <QMatrix>

#include <cmath>

APolygonTool::APolygonTool()
{
	
}


APolygonTool::~APolygonTool()
{
}

QHash< QString, DAction * > APolygonTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *action1 = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/")), tr("Polygon"), this);
	action1->setShortcut( QKeySequence( tr("") ) );
	
	hash.insert(tr("Polygon"), action1);
	return hash;
}

QPainterPath APolygonTool::path() const
{
	return m_shape;
}

QRect APolygonTool::move(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& oldPos, const QPoint& newPos)
{
	emit toDrawGhostGraphic(createShape(QRect(m_initialPoint, newPos)));
	return QRect(0,0,0,0);
}

QRect APolygonTool::press(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos, KTKeyFrame* currentFrame)
{
	m_initialPoint = pos;
	return QRect(0,0,0,0);
}

QRect APolygonTool::release(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos)
{
	m_shape = createShape(QRect(m_initialPoint, pos));
	
	return QRect(m_initialPoint, pos);
}

QStringList APolygonTool::keys() const
{
	return QStringList() << "Polygon";
} 

QWidget* APolygonTool::configurator()
{
	return 0;
}

bool APolygonTool::isComplete() const
{
	return true;
}

int APolygonTool::type() const
{
	return Brush;
}

void APolygonTool::aboutToChangeTool()
{
}


QPainterPath APolygonTool::createShape(const QRect & rect)
{
	QPainterPath shape;
	
	double angle = 360/3 /*m_angle*/;

	// A star should have at least 3 edges:
	int m_edges = 5;
	int m_roundness = 10.0;
	// Make sure, radii are positive:
	int m_outerRadius = rect.width();
	int  m_innerRadius = rect.height();
	QPoint p2, p3;
	QPoint p( m_outerRadius * cos( angle + 3.1416),m_outerRadius * sin( angle + 3.1416 ) );
	m_shape.moveTo( p );

	double inAngle = 3.1416*2 / 360 * m_innerRadius;
	
	double innerRoundness = ( 3.1416 * m_innerRadius * m_roundness ) / m_edges;
	double outerRoundness = ( 3.1416 * m_outerRadius * m_roundness ) / m_edges;
	
	for ( uint i = 0; i < m_edges; ++i )
	{
		double nextOuterAngle = angle + 3.1416 + 3.1416 / m_edges * ( i + 1.0 );
		double nextInnerAngle = angle + inAngle + 3.1416 + 3.1416 / m_edges * ( i + 0.5 );

		p.setX( m_outerRadius * cos( nextOuterAngle ) );
		p.setY( m_outerRadius * sin( nextOuterAngle ) );
		int m_roundness = 0;
		
		if( m_roundness == 0.0 )
		{
			shape.lineTo( p );
		}
		else
		{
			p2.setX( m_innerRadius * cos( nextInnerAngle ) -
					cos( angle + inAngle + 3.1416 / m_edges * ( i + 0.5 ) ) * innerRoundness );
			p2.setY( m_innerRadius * sin( nextInnerAngle ) -
					sin( angle + inAngle + 3.1416 / m_edges * ( i + 0.5 ) ) * innerRoundness );
		
			p3.setX( m_outerRadius * cos( nextOuterAngle ) +
					cos( angle + 3.1416 / m_edges * ( i + 1.0 ) ) * outerRoundness );
			p3.setY( m_outerRadius * sin( nextOuterAngle ) +
					sin( angle + 3.1416 / m_edges * ( i + 1.0 ) ) * outerRoundness );
		
			shape.cubicTo( p2, p3, p );
		}
	}
	
	QMatrix matrix;
	matrix.translate(rect.center().x(), rect.center().y());
	
	shape = matrix.map(shape);
	
	return shape;
}


Q_EXPORT_PLUGIN( APolygonTool );

