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

#include "aselectionplugin.h"

#include <QKeySequence>
#include <QDebug>

#include "ktapplication.h"

QStringList ASelectionPlugin::keys() const
{
	return QStringList() << tr("Selection");
}

QRect ASelectionPlugin::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos)
{
// 	m_path = QPainterPath();
// 	m_path.moveTo(pos);
// 	
// 	return move(brush, painter, form, pos, pos);
}

QRect ASelectionPlugin::move(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos)
{
// 	painter.save();
// 
// 	int rad = painter.pen().width() / 2;
// 	QRect boundingRect = QRect(oldPos, newPos).normalized().adjusted(-rad, -rad, +rad, +rad);
// 	
// 	QColor color = painter.pen().color();
// 	int thickness = painter.pen().width();
// 	QColor transparentColor(color.red(), color.green(), color.blue(), 0);
// 
// 	QPainterPath path;
// 	path.setFillRule( Qt::WindingFill );
// 	
// 	if (brush == tr("Shape brush") )
// 	{
// 		path.addPath(form);
// 	}
// 	
// 	m_path.addPath(path);
// 	m_path.setFillRule( Qt::WindingFill );
// 	
// 	painter.drawPath(path);
// 	
// 	boundingRect = path.boundingRect().toRect();
// 	
// 	painter.restore();
// 	return boundingRect;
}

QRect ASelectionPlugin::release(const QString &  brush ,QPainter &  painter , const QPainterPath &form, const QPoint &  pos )
{
// 	return QRect(0, 0, 0, 0);
}

QPainterPath ASelectionPlugin::path() const
{
	return m_path;
}

QHash<QString, QAction *> ASelectionPlugin::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *act = new QAction(QPixmap(KTOON_THEME_DIR+"/icons/selection.png"), tr("Selection"), this);
// 	act->setShortcut( QKeySequence(tr("R")) );
	hash.insert( tr("Selection"), act );
	
	return hash;
}

Q_EXPORT_PLUGIN( ASelectionPlugin )


