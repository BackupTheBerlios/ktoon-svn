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
#include "aerasertool.h"
#include "dglobal.h"

AEraserTool::AEraserTool()
{
}


AEraserTool::~AEraserTool()
{
}

QStringList AEraserTool::keys() const
{
	return QStringList() << tr("Node eraser");
}

QRect AEraserTool::press(const QString &brush, QPainter &painter,const QPoint &pos, KTKeyFrame *currentFrame)
{
	QRect rect(pos, QSize(15,15)); // FIXME: configure
	
	m_components = currentFrame->components();
	
	deleteNode( rect );
	
	return QRect();
}

QRect AEraserTool::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	QRect rect(newPos, QSize(15,15)); // FIXME: configure
	
	deleteNode( rect );
	return QRect();
}

QRect AEraserTool::release(const QString &  brush ,QPainter &  painter, const QPoint &  pos )
{
	
	return QRect();
}

QPainterPath AEraserTool::path() const
{
	return QPainterPath();
}

QHash<QString, DAction *> AEraserTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *eraserAction = new DAction( QPixmap(THEME_DIR+"/icons/node_eraser.png"), tr("Node eraser"), this);
	eraserAction->setShortcut( QKeySequence(tr("T")) );
	
	hash.insert( tr("Node eraser"), eraserAction );
	
	return hash;
}

void AEraserTool::deleteNode(const QRect &rect )
{
	foreach(AGraphicComponent *component, m_components)
	{
		foreach(AGraphic *graphic, component->graphics() )
		{
			QList<QPolygonF> polygons = graphic->path.toSubpathPolygons();
			
			graphic->path = QPainterPath();
			
			foreach(QPolygonF polygon, polygons)
			{
				QPolygonF::iterator point = polygon.begin();
				
// 				bool contains = false;
				
				while(point != polygon.end())
				{
					if ( rect.contains( (*point).toPoint() ) )
					{
// 						contains = true;
						point=polygon.erase(point);
					}
					else
						++point;
				}
				
// 				if ( contains )
				graphic->path.addPolygon(polygon);
			}
		}
	}
	
	emit requestRedraw();
}

int AEraserTool::type() const
{
	return Brush;
}

QWidget *AEraserTool::configurator()
{
	return 0;
}

bool AEraserTool::isComplete() const
{
	return true;
}

void AEraserTool::aboutToChangeTool() 
{
}

Q_EXPORT_PLUGIN( AEraserTool );
