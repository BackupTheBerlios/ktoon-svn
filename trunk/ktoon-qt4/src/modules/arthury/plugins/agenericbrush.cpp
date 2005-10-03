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

QStringList AGenericBrush::keys() const
{
	return QStringList() << tr("Generic Brush");
}

QRect AGenericBrush::press(const QString &brush, QPainter &painter, const QPoint &pos)
{
	return move(brush, painter, pos, pos);
}

QRect AGenericBrush::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();

	int rad = painter.pen().width() / 2;
	QRect boundingRect = QRect(oldPos, newPos).normalized()
			.adjusted(-rad, -rad, +rad, +rad);
	QColor color = painter.pen().color();
	int thickness = painter.pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);

	if (brush == tr("Generic Brush") ) 
	{
		painter.drawLine(oldPos, newPos);
	}
	
	painter.restore();
	return boundingRect;
}

QRect AGenericBrush::release(const QString & /* brush */,QPainter & /* painter */, const QPoint & /* pos */)
{
	return QRect(0, 0, 0, 0);
}

QPixmap AGenericBrush::pixmap() const
{
	return QPixmap(brush_xpm);
}

Q_EXPORT_PLUGIN( AGenericBrush )

