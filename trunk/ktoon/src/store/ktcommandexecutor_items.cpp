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

#include "ktpathitem.h"
#include "ktrectitem.h"
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

#include <dcore/ddebug.h>

#include <QGraphicsItem>

bool KTCommandExecutor::createItem(KTItemResponse *response)
{
	D_FUNCINFOX("items");
// 	response->setAction(KTProjectRequest::Add);
	
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
					response->setItemIndex( frame->indexOf( item ));
				}
			}
			else
			{
				dError() << tr("Frame doesn't exists!");
				return false;
			}
		}
		else
		{
			dError() << tr("Layer doesn't exists!");
			return false;
		}
	}
	else
	{
		dError() << tr("Scene doesn't exists!");
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

bool KTCommandExecutor::removeItems(KTItemResponse *response) // FIXME: no es estandar.
{
	D_FUNCINFOX("items");
	
	response->setAction(KTProjectRequest::Remove);
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	
	QString strList = response->arg().toString();
	
	dFatal() << "removeItems = " << strList;
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPosition );
		if ( layer )
		{
			KTFrame *frame = layer->frame( framePosition );
			if ( frame )
			{
// 				QStringList infoItems;
// 				QDomDocument doc;
// 				doc.setContent(xml);
				
// 				if ( position == -1)
// 				{
// 					qFatal("DO NOT SEND -1");
// 				}
				
// 				QDomElement root = doc.documentElement();
// 				QString strList = root.attribute ( "positions");
				
				QString::const_iterator itr = strList.constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				qSort(positions.begin(), positions.end());
				
				int count = 0;
				
				foreach(qreal pos, positions )
				{
					int pstn = (int) pos;
					
					QGraphicsItem *item = frame->item(pstn-count);
// 					QDomDocument orig;
					
					if(item)
					{
// 						orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
// 						infoItems << orig.toString();
						frame->removeGraphicAt(pstn-count);
						
						count++;
					}
				}
				
				emit responsed(response);
				
				return true;
			}
		}
	}
	
	return false;
}

bool KTCommandExecutor::groupItems(KTItemResponse *response)
{
	D_FUNCINFOX("items");
// 	response->setAction(KTProjectRequest::Group);
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
	QString strList = response->arg().toString();
	SHOW_VAR(strList);
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
		
				response->setItemIndex( frame->indexOf( frame->createItemGroupAt( position, positions)));
				
				emit responsed(response);
				
				
				return true;
			}
		}
	}
	
	return false;
}

bool KTCommandExecutor::ungroupItems(KTItemResponse *response)
{
	D_FUNCINFOX("items");
	
// 	response->setAction(KTProjectRequest::Ungroup);
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int framePosition = response->frameIndex();
	int position = response->itemIndex();
// 	QString strList = response->arg().toString();
	
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
// 				QString::const_iterator itr = strList.constBegin();
// 				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
// 				foreach(qreal pos, positions)
// 				{
// 					int posint = (int)(pos);
				QList<QGraphicsItem *> items = frame->destroyItemGroup(position);
				foreach(QGraphicsItem *item, items)
				{
					if(frame->indexOf(item) != -1 )
					{
						if(strItems.isEmpty())
						{
							strItems +="("+ QString::number(frame->indexOf(item)) ;
// 							firstItem = currentScene->currentFrame()->indexOf(item);
						}
						else
						{
							strItems += " , "+ QString::number(frame->indexOf(item));
						}
					}
				}
				strItems+= ")";
// 				}
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
		case 2: // Path
		{
			KTPathItem *path = KTItemConverter::convertToPath( item );
			return path;
		}
		break;
		case 3: // Rect
		{
			KTRectItem *rect = KTItemConverter::convertToRect( item );

			return rect;
		}
		break;
		case 4: // Ellipse
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
		default:
		{
			dWarning() << "unknown item " << toType ;
		}
		break;
	}
	return 0;
}

bool KTCommandExecutor::convertItem(KTItemResponse *response)
{
	D_FUNCINFOX("items");
	
	response->setAction(KTProjectRequest::Convert);
	
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
// 					QDomDocument doc;
// 					if ( ! doc.setContent( xml ) ) return false;
// 					int toType = doc.documentElement().attribute( "type").toInt();
					
					dFatal() << KTProxyItem::Type << "==" << item->type();
					
					if ( toType == item->type() ) return false;
					
					QGraphicsItem * itemConverted = convert(item, toType);
					
					if(itemConverted)
					{
// 						scene->removeItem(item); // FIXME?
// 						scene->addItem(itemConverted); // FIXME?
						
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
	D_FUNCINFOX("items");
	response->setAction(KTProjectRequest::Transform);
	
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
	D_FUNCINFOX("items");
	response->setAction(KTProjectRequest::EditNodes);
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
	D_FUNCINFOX("items");
	response->setAction(KTProjectRequest::Tweening);
	
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
				
				QDomDocument doc;
				if( doc.setContent(xml) )
				{
					QVector<KTTweenerStep *> steps;
					
					QDomElement root = doc.documentElement();
					
					QDomNode n = root.firstChild();
					
					while( !n.isNull() )
					{
						QDomElement e = n.toElement();
						
						if(!e.isNull())
						{
							if( e.tagName() == "step" )
							{
								QDomDocument stepDoc;
								stepDoc.appendChild(stepDoc.importNode(n, true));
								KTTweenerStep *step = new KTTweenerStep(0);
								step->fromXml(stepDoc.toString(0));
								
								steps << step;
							}
						}
						
						n = n.nextSibling();
					}
					
					int frames = root.attribute("frames").toInt();
					
					if( frames < steps.count() )
					{
						frames = steps.count();
					}
					
					KTItemTweener *tweener = new KTItemTweener( frames, object);
					
					foreach(KTTweenerStep *step, steps)
					{
						dDebug() << step->n() << " " << step->position();
						tweener->addStep(*step);
					}
					
					object->setTweener(tweener);
					
					qDeleteAll(steps);
					steps.clear();
					
					return true;
				}
			}
		}
	}
	
	return false;
}


