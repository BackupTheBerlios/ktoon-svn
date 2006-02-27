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
#include "ktdebug.h"
#include <QPointF>
#include <QKeySequence>

#include "ktapplication.h"
#include "ktgraphicalgorithm.h"

QStringList AGenericBrush::keys() const
{
	return QStringList() << tr("Pencil") << tr("Air Brush") << tr("Bezier Brush");
}

QRect AGenericBrush::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos, KTKeyFrame *currentFrame )
{
	m_firstPoint = QPoint(0,0);
	m_path = QPainterPath();
	m_path.moveTo(pos);
	
	return move(brush, painter, form, pos, pos);
}

QRect AGenericBrush::move(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos)
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

QRect AGenericBrush::release(const QString & brush ,QPainter & /* painter*/,const QPainterPath &/*form*/, const QPoint & /* pos */)
{
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
	ktDebug() << pol.count();
	if (brush == tr("Pencil") )
	{
		m_path = QPainterPath();
		m_path.addPolygon(pol);
	}
	else if ( brush == tr("Bezier Brush"))
	{
		m_path = bezierFit(pol, 10);//FIXME cofigure
		emit  requestRedraw();
	}
	
	return QRect(0, 0, 0, 0);
}

QPainterPath AGenericBrush::path() const
{
	return m_path;
}

QHash<QString, KTAction *> AGenericBrush::actions()
{
	QHash<QString, KTAction *> hash;
	
	KTAction *pencil = new KTAction( QIcon(brush_xpm), tr("Pencil"), this);
	pencil->setShortcut( QKeySequence(tr("P")) );
	
	QPixmap pix(KTOON_THEME_DIR+"/cursors/pencil.png");
	pencil->setCursor( QCursor(pix, 0, pix.height()) );
	
	hash.insert( tr("Pencil"), pencil );
	
	KTAction *quadBrush = new KTAction( QIcon(), tr("Bezier Brush"), this);
	quadBrush->setShortcut( QKeySequence(tr("Q")) );
	hash.insert(tr("Bezier Brush"), quadBrush);
	
	return hash;
}

int AGenericBrush::type() const
{
	return Brush;
}
		
QWidget *AGenericBrush::configurator()
{
	return 0;
}
		
bool AGenericBrush::isComplete() const
{
	return true;
}

void AGenericBrush::aboutToChangeTool() 
{
}


Q_EXPORT_PLUGIN( AGenericBrush )

