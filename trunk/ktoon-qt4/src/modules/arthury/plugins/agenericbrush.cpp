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
#include "../../../images/icons/brush.xpm"
// #include "ktdebug.h"
#include <QDebug>

#include <QKeySequence>

QStringList AGenericBrush::keys() const
{
	return QStringList() << tr("Pencil") << tr("Air Brush") << tr("Quad Brush");
}

QRect AGenericBrush::press(const QString &brush, QPainter &painter, const QPoint &pos)
{
	return move(brush, painter, pos, pos);
}

QRect AGenericBrush::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();

	int rad = painter.pen().width() / 2;
	QRect boundingRect = QRect(oldPos, newPos).normalized().adjusted(-rad, -rad, +rad, +rad);
	QColor color = painter.pen().color();
	int thickness = painter.pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);

	if (brush == tr("Pencil") ) 
	{
		QPainterPath path;
		path.moveTo(oldPos);
		path.lineTo(newPos);
// 		painter.drawLine(oldPos, newPos);
		painter.drawPath(path);
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

			painter.drawEllipse(x - (thickness / 2), y - (thickness / 2),thickness, thickness);
		}
	}
	else if ( brush == tr("Quad Brush"))
	{
		painter.drawRect(newPos.x(), newPos.y(), 20, 20);
		boundingRect = QRect( newPos.x()-rad*2, newPos.y()-rad*2, 20+painter.pen().width()*2, 20+painter.pen().width()*2);
	}
	
	painter.restore();
	return boundingRect;
}

QRect AGenericBrush::release(const QString & /* brush */,QPainter & /* painter */, const QPoint & /* pos */)
{
	return QRect(0, 0, 0, 0);
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
	
	QAction *quadBrush = new QAction( QIcon(), tr("Quad Brush"), this);
	quadBrush->setShortcut( QKeySequence(tr("Q")) );
	hash.insert(tr("Quad Brush"), quadBrush);
	
	return hash;
}

Q_EXPORT_PLUGIN( AGenericBrush )

