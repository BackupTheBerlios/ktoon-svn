/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "afilltool.h"
#include "dglobal.h"

#include "dbrushadjuster.h"

#include <ddebug.h>

AFillTool::AFillTool()
{
}


AFillTool::~AFillTool()
{
}


QStringList AFillTool::keys() const
{
	return QStringList() << tr("Fill") << tr("Remove Fill") << tr("Countour Fill");
}


QHash< QString, DAction * > AFillTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *fillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/fill.png")), tr("Fill"), this);
	fillAction->setShortcut( QKeySequence( tr("F") ) );
	
	hash.insert( tr("Fill"), fillAction );
	
	DAction *removeFillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/removefill.png")), tr("Remove Fill"), this);
	removeFillAction->setShortcut( QKeySequence(tr("Shift+F") ));
	
	hash.insert( tr("Remove Fill"), removeFillAction );
	
	DAction *countourFillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/contour.png")), tr("Countour Fill"), this);
	countourFillAction->setShortcut( QKeySequence(tr("Control+F") ));
	
	hash.insert( tr("Countour Fill"), countourFillAction );
	
	return hash;
}

QPainterPath AFillTool::path() const
{
	return QPainterPath();
}

QRect AFillTool::press(const QString& brush, QPainter& painter, const QPoint& pos, KTKeyFrame* currentFrame)
{
	if( currentFrame->selectedComponents().count() > 0 )
	{
		AGraphicComponent *selected = currentFrame->selectedComponents()[0];
		
		QList<AGraphicComponent*> components = QList<AGraphicComponent*>() << selected << selected->allChilds();
		
		bool finded = false;
		
		AGraphic *current = 0;
		
		QList<AGraphic *> allGraphics;
		
		foreach(AGraphicComponent *component, currentFrame->components())
		{
			foreach(AGraphic *graphic, component->graphics())
			{
				if ( graphic->path.contains(pos) )
				{
					current = graphic;
					finded = true;
#if 1
					if ( brush == tr("Remove Fill") )
					{
						graphic->brush = Qt::transparent;
						emit requestRedraw();
						return QRect(0,0,0,0);
					}
					else if ( brush == tr("Countour Fill") )
					{
						graphic->pen.setBrush(painter.pen().brush());
						emit requestRedraw();
						return QRect(0,0,0,0);
					}
#endif
					break;
				}
			}
			
			if ( finded )
			{
				break;
			}
		}
		
		
		if ( current )
		{
			foreach(AGraphicComponent *component, currentFrame->components())
			{
				foreach(AGraphic *graphic, component->graphics())
				{
					if ( graphic != current )
					{
						if ( graphic->path.intersects(current->path.boundingRect()) )
						{
							allGraphics << graphic;
						}
					}
				}
			}
			
// 			SHOW_VAR(allGraphics.count());
			
			AGraphicComponent *component = new AGraphicComponent;
			
			QPainterPath fillPath;
			
			buildPath(current, fillPath, allGraphics);
			
			component->addGraphic( fillPath, Qt::NoPen, DBrushAdjuster::adjustBrush(painter.pen().brush(), fillPath.boundingRect().toRect()) );
			
			int index = currentFrame->components().indexOf(selected)+1;
			
			if ( index > currentFrame->components().count() )
			{
				index = currentFrame->components().count()-1;
			}
			
			currentFrame->insertComponent(index, component );
		}
		
		if ( finded)
		{
			emit requestRedraw();
		}
	}
	
	return QRect(0,0,0,0);
}

// 	if ( current->path.intersects( QRectF(QPointF(graphic->path.elementAt(0).x - offset/2,graphic->path.elementAt(0).y - offset/2 ), QSizeF(offset,offset) )) || current->path.intersects( QRectF(QPointF(graphic->path.elementAt(graphic->path.elementCount()-1).x - offset/2, graphic->path.elementAt(graphic->path.elementCount()-1).y - offset/2), QSizeF(offset,offset)) ))

void AFillTool::buildPath(AGraphic *current, QPainterPath &fillPath, QList<AGraphic *> &graphics)
{
	QPolygonF polygon = current->path.toFillPolygon();
	
	foreach(AGraphic *graphic, graphics)
	{
		if ( current->path.contains(graphic->path.boundingRect()) )
		{
			if ( isClosed(graphic->path) )
			{
				fillPath.addPath(graphic->path);
			}
		}
		else
		{
			for(int i = 0; i < polygon.count(); i++)
			{
				if ( isContained(polygon[i], graphic->path) )
				{
					polygon.remove(i);
				}
			}
			
			QPolygonF graphicPol = graphic->path.toFillPolygon();
			QPolygonF newPol;
			for(int point = 0; point < graphicPol.count(); point++)
			{
				if ( isIntersected(graphicPol[point], polygon ))
				{
					newPol << graphicPol[point];
				}
			}
			if ( newPol.count() > 0 )
			{
				fillPath.addPolygon(newPol);
// 				fillPath.closeSubpath();
			}
		}
	}
	
	fillPath.addPolygon(polygon);
// 	fillPath.closeSubpath();
	
	return;
}

bool AFillTool::isIntersected(const QPointF &point, const QPolygonF &polygon)
{
	QPainterPath tmp;
	tmp.addPolygon(polygon);
	int offset = 1;
		
	return tmp.intersects(QRectF(QPointF(point.x()-offset/2, point.y()-offset/2), QSizeF(offset,offset)));
}

bool AFillTool::isContained(const QPointF &point, const QPainterPath &path)
{
	int offset = 2;
	
	return path.contains(QRectF(QPointF(point.x()-offset/2, point.y()-offset/2), QSizeF(offset,offset)));
	
// 	return path.contains(point);
}

bool AFillTool::isClosed(const QPainterPath &path)
{
	// FIXME: verify internal intersect
	int offset = 5;
	
	QRectF rect(QPointF(path.elementAt(0).x-offset/2, path.elementAt(0).y-offset/2), QSizeF(offset,offset));
	
	return rect.contains( path.elementAt(path.elementCount()-1) );
}


void AFillTool::joinPath(QPainterPath &orig, const QPainterPath &toJoin)
{
// 	QPolygonF newPol;
// 	
// 	QList<QPolygonF> polygons = toJoin.toSubpathPolygons();
// 	
// 	foreach(QPolygonF pol, polygons)
// 	{
// 		newPol << pol;
// 	}
// 	
// 	orig.addPolygon(newPol);
	orig.connectPath(toJoin);
}

QRect AFillTool::move(const QString& brush, QPainter& painter, const QPoint& oldPos, const QPoint& newPos)
{
	return QRect(0,0,0,0);
}

QRect AFillTool::release(const QString &brush, QPainter &painter,const QPoint &pos)
{
	
	return QRect(0,0,0,0);
}

QWidget * AFillTool::configurator()
{
	return 0;
}

int AFillTool::type() const
{
	return Fill;
}

bool AFillTool::isComplete() const
{
	return false;
}
void AFillTool::aboutToChangeTool() 
{
}

Q_EXPORT_PLUGIN( AFillTool );


