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

#include "ktapplication.h"
#include "ktdebug.h"

QStringList ASelectionPlugin::keys() const
{
	return QStringList() << tr("Selection");
}

QRect ASelectionPlugin::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos, KTKeyFrame *currentFrame )
{
	QRect rect;
	if ( currentFrame  )
	{
		if(currentFrame->selectedComponents().count() > 0)
		{
			m_graphics = currentFrame->selectedComponents();
			rect = drawControls(&painter);
		}
	}
// 	m_currentGraphic = clickedGraphic;
	
	return rect;
}

QRect ASelectionPlugin::move(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos)
{
	QRectF boundingRect;
	if ( m_graphics.count() > 0 )
	{
		QPainterPath ghost;
		
		QMatrix matrix;
		matrix.translate(newPos.x()-oldPos.x(), newPos.y()-oldPos.y());
		
		foreach(AGraphicComponent *selected, m_graphics )
		{
			ghost.addPath(selected->path());
			selected->setPath(matrix.map(selected->path()));
		}
		
		int rad = painter.pen().width();
		
		ghost = matrix.map(ghost);
		boundingRect = ghost.boundingRect().normalized().adjusted(-rad, -rad, +rad, +rad);
		emit toDrawGhostGraphic( ghost );
	}
	return boundingRect.toRect();
}

QRect ASelectionPlugin::release(const QString &  brush ,QPainter &  painter , const QPainterPath &form, const QPoint &  pos )
{
	emit requestRedraw();
	QRect rect = drawControls(&painter);;
	
	return rect;
}

QPainterPath ASelectionPlugin::path() const
{
	return m_path;
}

QHash<QString, QAction *> ASelectionPlugin::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *act = new QAction(QPixmap(KTOON_THEME_DIR+"/icons/selection.png"), tr("Selection"), this);
	hash.insert( tr("Selection"), act );
	
	return hash;
}

QRect ASelectionPlugin::drawControls(QPainter *painter)
{
	QRect rect;
	foreach(AGraphicComponent *clickedGraphic, m_graphics)
	{
		if ( clickedGraphic )
		{
			rect = clickedGraphic->boundingRect().toRect().unite(rect);
			QPainterPath path;
				
			path.addRect( QRectF(rect.bottomLeft() - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF(rect.bottomRight() - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF(rect.topLeft() - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF(rect.topRight() - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF(rect.center() - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF( QPointF(rect.x(), rect.y()+rect.height()/2 ) - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF( QPointF(rect.x()+rect.width(), rect.y()+rect.height()/2 ) - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF( QPointF(rect.x()+rect.width()/2, rect.y() ) - QPointF(2, 2), QSizeF(4,4)));
			path.addRect( QRectF( QPointF(rect.x()+rect.width()/2, rect.y()+rect.height() ) - QPointF(2, 2), QSizeF(4,4)));
			painter->save();
			painter->setPen(QColor("blue"));
			painter->setBrush(QColor("blue"));
			painter->drawPath(path);
			painter->restore();
		}
	}
	
	return rect;
}

Q_EXPORT_PLUGIN( ASelectionPlugin )


