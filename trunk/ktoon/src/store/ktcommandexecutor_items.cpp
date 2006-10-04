/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   m_project program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   m_project program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with m_project program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ktcommandexecutor.h"

#include "ktserializer.h"
#include "ktscene.h"
#include "ktitemrequest.h"
#include "ktpathitem.h"
#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktitemconverter.h"
#include "ktsvg2qt.h"
#include "ktitemfactory.h"

#include <dcore/ddebug.h>

#include <QGraphicsItem>

QString KTCommandExecutor::createItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	D_FUNCINFO;
	KTScene *scene = m_project->scene(scenePosition);
	
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
					KTItemRequest event(KTProjectRequest::Add, scenePosition, layerPosition, framePosition, position, xml);
					emit commandExecuted(&event, m_isRedo);
				}
			}
			else
			{
				dError() << tr("Frame doesn't exists!");
			}
		}
		else
		{
			dError() << tr("Layer doesn't exists!");
		}
	}
	else
	{
		dError() << tr("Scene doesn't exists!");
	}
	
	return QString();
}


QString KTCommandExecutor::removeItem(int scenePosition, int layerPosition, int framePosition, int position)
{
	D_FUNCINFO;
	
	KTScene *scene = m_project->scene(scenePosition);
	
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
					KTItemRequest event(KTProjectRequest::Remove, scenePosition, layerPosition, framePosition, position, 0);
					emit commandExecuted(&event, m_isRedo);
				}
			}
		}
	}
	
	return QString();
}

QStringList KTCommandExecutor::removeItems(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	D_FUNCINFO;
	KTScene *scene = m_project->scene(scenePosition);
	
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
				
				QStringList infoItems;
				QDomDocument doc;
				doc.setContent(xml);
				
				QDomElement root = doc.documentElement();
				QString strList = root. attribute ( "positions"); 
				
				QString::const_iterator itr = strList.constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				qSort(positions.begin(), positions.end());
// 				dDebug() << positions;
				int count = 0;
				foreach(qreal pos, positions )
				{
					QGraphicsItem *item = frame->item((int)pos-count);
					QDomDocument orig;
					
					if(item)
					{
						orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
						infoItems << orig.toString();
						frame->removeItemAt((int)pos-count);
						
						count++;
					}
				}
				
				KTItemRequest event(KTProjectRequest::Remove, scenePosition, layerPosition, framePosition, position, strList);
				emit commandExecuted(&event, m_isRedo);
				
				return infoItems;
			}
		}
	}
	
	return QStringList();
}

QStringList KTCommandExecutor::groupItems(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml )
{
	D_FUNCINFO << xml;
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
// 				if ( position == -1 )
// 				{
// 					position = frame->graphics().count() - 1;
// 				}
				QStringList infoItems;
				QDomDocument doc;
				doc.setContent(xml);
				QDomElement root = doc.documentElement();
				QString strList = root.attribute ( "positions");
				
				QString::const_iterator itr = strList.constBegin();
				
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				
// 				dDebug() << positions;
// 				int count = 0;
// 				foreach(qreal pos, positions )
// 				{
// 					QGraphicsItem *item = frame->item(pos-count);
// 					QDomDocument orig;
// 					
// 					if(item)
// 					{
// 						orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
// 						infoItems << orig.toString();
// 						
// 						count++;
// 					}
// 				}
				frame->createItemGroupAt( position, positions);
				
				KTItemRequest event(KTProjectRequest::Group, scenePosition, layerPosition, framePosition, position, xml);
				emit commandExecuted(&event, m_isRedo);
				
				return infoItems;
			}
		}
	}
	
	return QStringList();
}

QString KTCommandExecutor::convertItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	D_FUNCINFO; 
	KTScene *scene = m_project->scene(scenePosition);
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
						case 2: // Path
						{
							KTPathItem *path = KTItemConverter::convertToPath( item );
							scene->addItem(path);
							
							frame->replaceGraphic(position, path);
						}
						break;
						case 3: // Rect
						{
							KTRectItem *rect = KTItemConverter::convertToRect( item );
							scene->addItem(rect);
							
							frame->replaceGraphic(position, rect);
						}
						break;
						case 4: // Ellipse
						{
							KTEllipseItem *ellipse = KTItemConverter::convertToEllipse( item );
							
							scene->addItem(ellipse);
							
							frame->replaceGraphic(position, ellipse);
						}
						break;
						
					}
					
					
					KTItemRequest event(KTProjectRequest::Convert, scenePosition, layerPosition, framePosition, position, xml);
					
					emit commandExecuted(&event, m_isRedo);
					
					return "<convert type=\""+QString::number(item->type())+"\" />";
				}
			}
		}
	}
	
	return QString();
}

QString KTCommandExecutor::transformItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml)
{
	KTScene *scene = m_project->scene(scenePosition);
	
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
					
					
					KTItemRequest event(KTProjectRequest::Transform, scenePosition, layerPosition, framePosition, position, xml);
					
					emit commandExecuted(&event, m_isRedo);
					
					return current;
				}
			}
		}
	}
	
	return "";
}

QString KTCommandExecutor::setPathItem( int scenePosition, int layerPosition, int framePosition, int position, const QString &xml )
{
	KTScene *scene = m_project->scene(scenePosition);
	
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
// 					
					if(qgraphicsitem_cast<QGraphicsPathItem *>(item));
					{
						QDomDocument orig;
						
						orig.appendChild(qgraphicsitem_cast<KTPathItem*>(item)->toXml(orig));
						
						QString current = orig.toString();
						QDomDocument doc;
						doc.setContent( xml);
						
						KTItemFactory factory;
						factory.loadItem(item, xml);
						KTItemRequest event(KTProjectRequest::EditNodes,	scenePosition, layerPosition, framePosition, position, xml);
						emit commandExecuted(&event, m_isRedo);
						return current;
					}
				}
			}
		}
	}
	return QString();
}

