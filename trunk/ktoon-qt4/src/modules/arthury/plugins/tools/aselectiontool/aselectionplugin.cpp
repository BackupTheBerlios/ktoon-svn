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
	return QStringList() << tr("Selection") << tr("Contour");
}

QRect ASelectionPlugin::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos, KTKeyFrame *currentFrame )
{
	m_node.componentpos = 0;
	m_node.graphicPos = 0;
	m_node.polygonPos = 0;
	m_node.pointPos = 0;
	selectPoint = false;
	QRect rect;
	if ( currentFrame  )
	{
		if(currentFrame->selectedComponents().count() > 0)
		{
			m_graphics = currentFrame->selectedComponents();
			rect = drawControls(brush, &painter);
		}
		else
		{
			m_graphics.clear();
		}
	}
	
	if( brush == tr("Contour"))
	{
		int thickness = 5;
		QRectF rectS(pos-QPointF(thickness/2,thickness/2) , QSizeF(thickness,thickness)); // FIXME: configure
		foreach(AGraphicComponent *component, m_graphics)
		{
			foreach(AGraphic *graphic, component->graphics() )
			{
				QList<QPolygonF> polygons = graphic->path.toSubpathPolygons();
				ktDebug() << "polygons.count()" <<  polygons.count();
				foreach(QPolygonF polygon, polygons)
				{
					QPolygonF::iterator point = polygon.begin();
					while(point != polygon.end())
					{
						if ( rectS.contains( (*point).toPoint() ) )
						{
							selectPoint = true;
							break;
						}
						m_node.pointPos++;
						++point;
					}
					
					if(selectPoint)
					{
						ktDebug() << m_node.polygonPos;
						break;
					}
					m_node.polygonPos++;
					m_node.pointPos = 0;
				}
				if(selectPoint)
				{
					break;
				}
				m_node.graphicPos++;
			}
			if(selectPoint)
			{
				break;
			}
			m_node.componentpos++;
		}
	}
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
		if(brush == tr("Selection"))
		{
			foreach(AGraphicComponent *selected, m_graphics )
			{
				foreach(AGraphic *graphic, selected->graphics() )
				{
					
					ghost.addPath(graphic->path );
					graphic->path = matrix.map(graphic->path);
				}
			}
		}
		else if(brush == tr("Contour") && selectPoint)
		{

			QList<QPolygonF> polygons = m_graphics[m_node.componentpos]->graphics()[m_node.graphicPos]->path.toSubpathPolygons();
			m_graphics[m_node.componentpos]->graphics()[m_node.graphicPos]->path = QPainterPath();
			foreach(QPolygonF polygon, polygons)
			{
				if(polygon == polygons[m_node.polygonPos])
				{
					if(m_node.pointPos == 0 && polygon.first () == polygon.last ()  )
					{
						polygon[m_node.pointPos] = newPos;
						polygon[polygon.count()-1] = newPos;
					}
					else
					{
						polygon[m_node.pointPos] = newPos;
					}
				}
				
				m_graphics[m_node.componentpos]->graphics()[m_node.graphicPos]->path.addPolygon(polygon);
				
			}
			
			ghost = m_graphics[m_node.componentpos]->graphics()[m_node.graphicPos]->path;
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
	m_node.componentpos = 0;
	m_node.graphicPos = 0;
	m_node.polygonPos = 0;
	m_node.pointPos = 0;
	emit requestRedraw();
	QRect rect = drawControls(brush, &painter);
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
	
	act = new QAction(QPixmap(KTOON_THEME_DIR+"/icons/nodes.png"), tr("Contour"), this);
	hash.insert( tr("Contour"), act );
	
	
	return hash;
}

QRect ASelectionPlugin::drawControls(const QString brush, QPainter *painter)
{
	QRect rect;
	foreach(AGraphicComponent *clickedGraphic, m_graphics)
	{
		if ( clickedGraphic )
		{
			rect = clickedGraphic->boundingRect().toRect().unite(rect);
		}
	}
	
	if( brush == tr("Selection") )
	{
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
	else if( brush == tr("Contour") )
	{
		painter->save();
		painter->setPen(QPen(Qt::red, 5 , Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); //FIXME configure
		foreach(AGraphicComponent *clickedGraphic, m_graphics)
		{
			foreach(AGraphic * graphic, clickedGraphic->graphics())
			{
				painter->drawPoints( graphic->path.toFillPolygon());
			}
		}
		painter->restore();
	}
	return rect;
}



Q_EXPORT_PLUGIN( ASelectionPlugin )


