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

QStringList AGenericBrush::keys() const
{
	return QStringList() << tr("Pencil") << tr("Air Brush") << tr("Bezier Brush");
}

QRect AGenericBrush::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos, AGraphicComponent *currentComponent)
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
	m_path.setFillRule ( Qt::WindingFill );
	if (brush == tr("Pencil") )
	{
		path.moveTo(oldPos);
		path.lineTo(newPos);
	}
	else if ( brush == tr("Air Brush"))
	{
		int numSteps = 2 + (newPos - oldPos).manhattanLength() / 2;

		painter.setBrush(QBrush(color, Qt::Dense6Pattern));
		painter.setPen(Qt::NoPen);

		for (int i = 0; i < numSteps; ++i)
		{
			int x = oldPos.x() + i * (newPos.x() - oldPos.x()) / (numSteps - 1);
			int y = oldPos.y() + i * (newPos.y() - oldPos.y()) / (numSteps - 1);

			path.addEllipse(x - (thickness / 2), y - (thickness / 2),thickness, thickness);
		}
	}
	else if ( brush == tr("Bezier Brush"))
	{
		if ( ! m_firstPoint.isNull() )
		{
			m_points << oldPos << newPos;
			path.moveTo(oldPos);
// 			for(int i = 0; i < poly.count() ; i+=2)
// 			{
				QPointF medio = ( oldPos + newPos)/2;
				QPointF cuarto = ( oldPos + medio)/2;
				path.cubicTo(oldPos, cuarto, medio);
				path.cubicTo(medio,  (cuarto + newPos)/2 , newPos);
// 			}
// 			path.moveTo(oldPos);
			
// 			path.cubicTo(oldPos, m_firstPoint, newPos);
// 			ktDebug() << newPos;
		}
		m_firstPoint = oldPos;
	}

	painter.drawPath(path);
	m_path.addPath ( path );
// 	m_path.connectPath ( path );
	
	painter.restore();
	return boundingRect;
}

QRect AGenericBrush::release(const QString & /* brush */,QPainter &  /*painter */,const QPainterPath &/*form*/, const QPoint & /* pos */)
{
	m_firstPoint = QPoint(0,0);
	return QRect(0, 0, 0, 0);
}

QPainterPath AGenericBrush::path() const
{
	return m_path;
}

QHash<QString, QAction *> AGenericBrush::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *pencil = new QAction( QIcon(brush_xpm), tr("Pencil"), this);
	pencil->setShortcut( QKeySequence(tr("P")) );
	
	hash.insert( tr("Pencil"), pencil );
	
	QAction *airBrush = new QAction( QIcon(), tr("Air Brush"), this);
	airBrush->setShortcut( QKeySequence(tr("A")) );
	hash.insert(tr("Air Brush"), airBrush);
	
	QAction *quadBrush = new QAction( QIcon(), tr("Bezier Brush"), this);
	quadBrush->setShortcut( QKeySequence(tr("Q")) );
	hash.insert(tr("Bezier Brush"), quadBrush);
	
	return hash;
}

Q_EXPORT_PLUGIN( AGenericBrush )

