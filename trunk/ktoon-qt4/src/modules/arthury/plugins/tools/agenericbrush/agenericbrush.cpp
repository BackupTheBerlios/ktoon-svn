/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "agenericbrush.h"
#include "brush.xpm"
#include "ddebug.h"
#include <QPointF>
#include <QKeySequence>

#include "dglobal.h"
#include "ktgraphicalgorithm.h"

AGenericBrush::AGenericBrush() : m_configurator(0)
{
	m_configurator = new ExactnessConfigurator;
}

AGenericBrush::~AGenericBrush()
{
	DEND;
	delete m_configurator;
}

QStringList AGenericBrush::keys() const
{
	return QStringList() << tr("Pencil") ;
}

QRect AGenericBrush::press(const QString &brush, QPainter &painter,const QPoint &pos, KTKeyFrame *currentFrame )
{
	m_firstPoint = pos;
	m_path = QPainterPath();
	m_path.moveTo(pos);
	return move(brush, painter, pos, pos);
}

QRect AGenericBrush::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();

	int rad = painter.pen().width();
	QRect boundingRect = QRect(oldPos, newPos).normalized().adjusted(-rad, -rad, +rad, +rad);
	QColor color = painter.pen().color();
	int thickness = painter.pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);
	
	QPainterPath path;
	path.setFillRule ( Qt::WindingFill );
// 	m_path.setFillRule ( Qt::WindingFill );
	path.moveTo(oldPos);
	path.lineTo(newPos);
	
	m_path.closeSubpath();
	m_path.addPath(path);

	painter.drawPath(path);
	
	painter.restore();
	return boundingRect;
}

QRect AGenericBrush::release(const QString & brush ,QPainter &  painter, const QPoint &  pos )
{
	int smoothness = m_configurator->exactness();
	
	if ( m_firstPoint == pos && m_path.elementCount() == 1)
	{
		smoothness = 0;
		m_path.addEllipse(pos.x(), pos.y(), painter.pen().width(),painter.pen().width());
		painter.drawPath(m_path);
	}
	
	m_firstPoint = QPoint(0,0);
	
	
	QPolygonF pol;
	QList<QPolygonF> polygons = m_path.toSubpathPolygons();
	
	QList<QPolygonF>::iterator it = polygons.begin();
	
	
	QPolygonF::iterator pointIt;
	while(it != polygons.end() )
	{
		pointIt = (*it).begin();
		
		while(pointIt <= (*it).end()-2)
		{
			pol << (*pointIt);
			pointIt += 2;
		}
		++it;
	}
	
	if(smoothness > 0)
	{
		m_path = KTGraphicalAlgorithm::bezierFit(pol, smoothness);
		emit requestRedraw();
		return QRect(0, 0, 0, 0);
	}
	else
	{
		m_path = QPainterPath();
		m_path.addPolygon(pol);
	}
	
	return m_path.boundingRect().toRect().normalized().adjusted(-painter.pen().width(), -painter.pen().width(), +painter.pen().width(), +painter.pen().width());;
}

QPainterPath AGenericBrush::path() const
{
	return m_path;
}

QHash<QString, DAction *> AGenericBrush::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *pencil = new DAction( QIcon(brush_xpm), tr("Pencil"), this);
	pencil->setShortcut( QKeySequence(tr("Ctrl+P")) );
	
	QPixmap pix(THEME_DIR+"/cursors/pencil.png");
	pencil->setCursor( QCursor(pix, 0, pix.height()) );
	
	hash.insert( tr("Pencil"), pencil );
	
	return hash;
}

int AGenericBrush::type() const
{
	return Brush;
}

QWidget *AGenericBrush::configurator() 
{
	if ( ! m_configurator )
	{
		m_configurator = new ExactnessConfigurator;;
	}
	
	return m_configurator;
}
		
bool AGenericBrush::isComplete() const
{
	return true;
}

void AGenericBrush::aboutToChangeTool() 
{
}


Q_EXPORT_PLUGIN( AGenericBrush )

