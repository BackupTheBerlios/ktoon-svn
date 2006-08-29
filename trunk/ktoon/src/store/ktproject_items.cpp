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

#include "ktproject.h"
#include "ktserializer.h"
#include "ktscene.h"
#include "ktitemevent.h"
#include "ktpathitem.h"
#include "ktrectitem.h"
#include "ktellipseitem.h"

#include <dcore/ddebug.h>

#include <QGraphicsItem>

QGraphicsItem *KTProject::createItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				if ( position == -1 )
				{
					position = frame->graphics().count() - 1;
				}
				
				QGraphicsItem *item = frame->createItem(position, xml);
				if ( item )
				{
					KTItemEvent event(KTProjectEvent::Add, scenePosition, layerPosition, framePosition, position, xml);
					emit commandExecuted( &event);
				}
				
				return item;
			}
		}
	}
	return 0;
}


void KTProject::removeItem(int scenePosition, int layerPosition, int framePosition, int position)
{
	D_FUNCINFO;
	KTScene *scene = this->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				if ( position == -1 )
				{
					position = frame->graphics().count() - 1;
				}
				
				if( frame->removeItemAt(position) )
				{
					KTItemEvent event(KTProjectEvent::Remove, scenePosition, layerPosition, framePosition, position, 0);
					emit commandExecuted( &event);
				}
			}
		}
	}
}

QString KTProject::convertItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	D_FUNCINFO;
	KTScene *scene = this->scene(scenePosition);
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				QGraphicsItem *item = frame->item(position);
				if ( item )
				{
					QDomDocument doc;
					if ( ! doc.setContent( xml ) ) return QString();
					int toType = doc.documentElement().attribute( "type").toInt();
					
					if ( toType == item->type() ) return QString();
					
					scene->removeItem(item);
					
					switch(toType)
					{
						case 2:
						{
							KTPathItem *tmp = new KTPathItem( item->parentItem(), scene);
							tmp->setPath(item->shape());
							tmp->setMatrix(item->matrix());
							tmp->setPos(item->scenePos());
							tmp->setFlags(item->flags() );
							if(QAbstractGraphicsShapeItem *shape =  qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item))
							{
								tmp->setBrush( shape->brush() );
								tmp->setPen(shape->pen() );
							}
							
							frame->replaceGraphic(position, tmp);
							
							tmp->setSelected(item->isSelected());
							//FIXME
		// 					item->setSelected(false);
		// 					delete item;
		// 					item = 0;
						}
						break;
						case 3: // Rect
						{
							KTRectItem *rect = new KTRectItem(item->parentItem(), scene);
							rect->setMatrix(item->matrix());
							
							switch(item->type() )
							{
								case 2:
								{
									rect->setRect(qgraphicsitem_cast<QGraphicsPathItem *>(item)->path().boundingRect());
								}
								break;
								case 4:
								{
									rect->setRect(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect());
								}
								break;
							}
							
							rect->setPos(item->scenePos());
							rect->setFlags(item->flags() );
							if(QAbstractGraphicsShapeItem *shape =  qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item))
							{
								rect->setBrush( shape->brush() );
								rect->setPen(shape->pen() );
							}
							
							frame->replaceGraphic(position, rect);
							
							rect->setSelected(item->isSelected());
						}
						break;
						case 4: // Ellipse
						{
							KTEllipseItem *ellipse = new KTEllipseItem(item->parentItem(), scene);
							ellipse->setMatrix(item->matrix());
							
							switch(item->type() )
							{
								case 2:
								{
									ellipse->setRect(qgraphicsitem_cast<QGraphicsPathItem *>(item)->path().boundingRect());
								}
								break;
								case 4:
								{
									ellipse->setRect(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect());
								}
								break;
							}
							
							ellipse->setPos(item->scenePos());
							ellipse->setFlags(item->flags() );
							if(QAbstractGraphicsShapeItem *shape =  qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item))
							{
								ellipse->setBrush( shape->brush() );
								ellipse->setPen(shape->pen() );
							}
							
							frame->replaceGraphic(position, ellipse);
							
							ellipse->setSelected(item->isSelected());
						}
						break;
						
					}
					
					
					KTItemEvent event(KTProjectEvent::Convert, scenePosition, layerPosition, framePosition, position, xml);
					
					emit commandExecuted( &event);
					
					return "<convert type=\""+QString::number(item->type())+"\" />";
				}
			}
		}
	}
	
	return QString();
}

QString KTProject::transformItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				QGraphicsItem *item = frame->item(position);
				if ( item )
				{
					QDomDocument orig;
					
					orig.appendChild(KTSerializer::properties( item, orig ));
					
					QString current = orig.toString();
					
					QDomDocument doc;
					doc.setContent( xml);
					KTSerializer::loadProperties( item, doc.documentElement());
					
					
					KTItemEvent event(KTProjectEvent::Transform, scenePosition, layerPosition, framePosition, position, xml);
					
					emit commandExecuted( &event);
					
					return current;
				}
			}
		}
	}
	
	return "";
}







