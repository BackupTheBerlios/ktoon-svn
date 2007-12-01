/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktcommandexecutor.h"

#include "ktserializer.h"
#include "ktscene.h"

#include "ktpathitem.h"
#include "ktrectitem.h"
#include "ktlineitem.h"
#include "ktellipseitem.h"
#include "ktitemconverter.h"
#include "ktsvg2qt.h"

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"
#include "ktitemfactory.h"
#include "ktprojectresponse.h"
#include "ktproxyitem.h"
#include "kttweenerstep.h"
#include "ktitemtweener.h"
#include "ktgraphicobject.h"
#include "ktlayer.h"

#include <kcore/kdebug.h>

#include <QGraphicsItem>

bool KTCommandExecutor::createItem(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	QString xml = response->arg().toString();
	
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
					qFatal("DO NOT SEND -1");
				}
				
				QGraphicsItem *item = frame->createItem(position, xml);
				if ( item )
				{
					emit responsed(response);
					response->setItemIndex( frame->visualIndexOf( item ));
				}
			}
			else
			{
				kError() << tr("Frame doesn't exists!");
				return false;
			}
		}
		else
		{
			kError() << tr("Layer doesn't exists!");
			return false;
		}
	}
	else
	{
		kError() << tr("Scene doesn't exists!");
		return false;
	}
	
	return true;
}

bool KTCommandExecutor::removeItem(KTItemResponse *response)
{
// 	response->setAction(KTProjectRequest::Remove);
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				QGraphicsItem *item = frame->item( response->itemIndex() );
				if( KTAbstractSerializable *itemSerializable = dynamic_cast<KTAbstractSerializable *>(item) )
				{
					QDomDocument orig;
					orig.appendChild(itemSerializable->toXml(orig));
					
					response->setArg(orig.toString());
					frame->removeGraphicAt(response->itemIndex());
				}
				else
				{
					return false;
				}
				
				emit responsed(response);
				
				return true;
			}
		}
	}
	
	return false;
}


bool KTCommandExecutor::moveItem(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	int newPositon = response->arg().toInt();
	if(response->mode() == KTProjectResponse::Undo)
	{
		position = newPositon;
		newPositon = response->itemIndex();
	}
	
	
	KTScene *scene = m_project->scene(scenePosition);
	
	
	if(scene)
	{
		KTLayer *layer = scene->layer(layerPosition);
		if(layer)
		{
			KTFrame *frame = layer->frame(framePosition);
			if(frame)
			{
				if(frame->moveItem(position, newPositon))
				{
					emit responsed(response);
					return true;
				}
			}
		}
	}
	return false;
}


bool KTCommandExecutor::groupItems(KTItemResponse *response)
{
	K_FUNCINFOX("items");
// 	response->setAction(KTProjectRequest::Group);
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	QString strList = response->arg().toString();
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				QString::const_iterator itr = strList.constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				response->setItemIndex( frame->visualIndexOf( frame->createItemGroupAt( position, positions)));
				
				emit responsed(response);
				return true;
			}
		}
	}
	
	return false;
}

bool KTCommandExecutor::ungroupItems(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				QString strItems = "";
				QList<QGraphicsItem *> items = frame->destroyItemGroup(position);
				foreach(QGraphicsItem *item, items)
				{
					if(frame->visualIndexOf(item) != -1 )
					{
						if(strItems.isEmpty())
						{
							strItems +="("+ QString::number(frame->visualIndexOf(item)) ;
						}
						else
						{
							strItems += " , "+ QString::number(frame->visualIndexOf(item));
						}
					}
				}
				strItems+= ")";
				response->setArg(strItems);
				emit responsed(response);
				return true;
			}
		}
	}
	return false;
}

static QGraphicsItem * convert(QGraphicsItem *item, int toType)
{
	switch(toType)
	{
		case KTPathItem::Type: // Path
		{
			KTPathItem *path = KTItemConverter::convertToPath( item );
			return path;
		}
		break;
		case KTRectItem::Type: // Rect
		{
			KTRectItem *rect = KTItemConverter::convertToRect( item );

			return rect;
		}
		break;
		case KTEllipseItem::Type: // Ellipse
		{
			KTEllipseItem *ellipse = KTItemConverter::convertToEllipse( item );
			return ellipse;
		}
		break;
		case KTProxyItem::Type:
		{
			return new KTProxyItem( item );
		}
		break;
		case KTLineItem::Type:
		{
			return KTItemConverter::convertToLine( item );
		}
		break;
		default:
		{
			kWarning() << "unknown item " << toType ;
		}
		break;
	}
	return 0;
}

bool KTCommandExecutor::convertItem(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	int toType = response->arg().toInt();
	
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
					kDebug("items") << item->type();
					
					if ( toType == item->type() ) return false;
					
					QGraphicsItem * itemConverted = convert(item, toType);
					
					if(itemConverted)
					{
// 					scene->removeItem(item); // FIXME?
// 					scene->addItem(itemConverted); // FIXME?
						itemConverted->setZValue(item->zValue());
						frame->replaceItem(position, itemConverted);
						
						response->setArg( QString::number(item->type()));
						emit responsed(response);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool KTCommandExecutor::transformItem(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	QString xml = response->arg().toString();
	
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
					
					
					emit responsed(response);
					
					response->setArg(current);
					
					return true;
				}
			}
		}
	}
	
	return false;
}

bool KTCommandExecutor::setPathItem( KTItemResponse *response )
{
	K_FUNCINFOX("items");
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	QString xml = response->arg().toString();
	SHOW_VAR(xml);
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
					if(qgraphicsitem_cast<QGraphicsPathItem *>(item));
					{
						QDomDocument orig;
						
						if(KTPathItem *ktpath = qgraphicsitem_cast<KTPathItem *>(item))
						{
							orig.appendChild(ktpath->toXml(orig));
						}
						QString current = orig.toString();
						SHOW_VAR(current);
						
						QDomDocument doc;
						doc.setContent( xml);
						KTItemFactory factory;
						factory.loadItem(item, xml);
						emit responsed(response);
						response->setArg(current);
						return true;
					}
				}
			}
		}
	}
	return false;
}


bool KTCommandExecutor::createTweening(KTItemResponse *response)
{
	K_FUNCINFOX("items");
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	
	QString xml = response->arg().toString();
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
				KTGraphicObject *object = frame->graphic(position);
				
				if( object == 0 ) return false;
				
				KTItemTweener *tweener = new KTItemTweener( 0, object);
				tweener->fromXml(xml);
				
				object->setTweener(tweener);
			}
		}
	}
	
	return false;
}


