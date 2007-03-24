/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                   *
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

#include "ktpaintarea.h"

#include <dcore/ddebug.h>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>
#include <QApplication>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QClipboard>
#include <QMenu>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"

#include "ktprojectresponse.h"

#include "ktpaintareaevent.h"
#include "ktpaintarearotator.h"
#include "ktimagedevice.h"
#include "ktgraphicsscene.h"

#include "dcore/dconfig.h"
#include "dgui/dapplication.h"

#include <cmath>

#include "kttextitem.h"

#include "librarydialog.h"
#include "ktlibraryobject.h"
#include "ktrequestbuilder.h"

#include <dgui/dosd.h>

#include "ktscene.h"
#include "ktproject.h"

struct KTPaintArea::Private
{
	KTProject *project;
	int currentSceneIndex;
	QStringList copiesXml;
};



KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : KTPaintAreaBase(parent), d(new Private)
{
	d->project = project;
	d->currentSceneIndex = 0;
	
	setCurrentScene( 0 );
	if ( scene() )
	{
		graphicsScene()->setCurrentFrame( 0, 0 );
	}
}

KTPaintArea::~KTPaintArea()
{
	delete d;
}

void KTPaintArea::setCurrentScene(int index)
{
	KTScene *sscene = d->project->scene(index);
	if ( sscene )
	{
		d->currentSceneIndex = index;
		graphicsScene()->setCurrentScene(sscene);
		
// 		if ( d->tool ) // FIXME
// 		{
// 			d->tool->init( this );
// 		}
	}
	else
	{
		setDragMode(QGraphicsView::NoDrag);
	}
}

void KTPaintArea::frameResponse(KTFrameResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			setCurrentScene( event->sceneIndex() );
			
			if ( !scene() ) return;
			
			KTGraphicsScene *sscene = graphicsScene();
			
			sscene->setCurrentFrame(event->layerIndex(), event->frameIndex());
			sscene->drawPhotogram(event->frameIndex());
			
// 			if ( event->action() == KTProjectRequest::Select ) // FIXME
// 			{
// 				if ( d->tool ) d->tool->init( this );
// 			}
			
			dDebug("paintarea") << "frame: " << event->frameIndex() << " " << "layer: " << event->layerIndex();
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTGraphicsScene *sscene = graphicsScene();
			
			if ( sscene->currentFrameIndex() == event->frameIndex() )
			{
				viewport()->update();
			}
		}
		break;
		default:
		{
		}
		break;
	}
	
// 	if ( d->tool ) // FIXME
// 	{
// 		d->tool->init( this );
// 	}
}

void KTPaintArea::layerResponse(KTLayerResponse *event)
{
	if ( !scene() ) return;
	
	KTGraphicsScene *sscene = graphicsScene();
	if( event->action() == KTProjectRequest::View)
	{
		sscene->setLayerVisible(event->layerIndex(), event->arg().toBool());
	}
	
	if ( event->action() != KTProjectRequest::Add ||  event->action() != KTProjectRequest::Remove )
	{
		graphicsScene()->drawCurrentPhotogram();
		viewport()->update(scene()->sceneRect().toRect() );
	}
	
	
	
}

void KTPaintArea::sceneResponse(KTSceneResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			
			setCurrentScene( event->sceneIndex() );
			
		}
		break;
		case KTProjectRequest::Remove:
		{
			if ( event->sceneIndex() == d->currentSceneIndex )
			{
				setCurrentScene( d->currentSceneIndex-1 );
			}
		}
		break;
		default: break;
	}
}


void KTPaintArea::itemResponse(KTItemResponse *event)
{
	switch(event->action())
	{
// 		case KTProjectRequest::Add:
// 		{
// 		}
// 		break;
// 		case KTProjectRequest::Remove:
// 		{
// 			
// 		}
// 		break;
		case KTProjectRequest::Transform:
		{
			viewport()->update();
		}
		break;
		default:
		{
			graphicsScene()->drawCurrentPhotogram();
			viewport()->update(scene()->sceneRect().toRect() );
		}
	}
	
	//if ( d->tool ) // FIXME
	//{
	//	d->tool->itemResponse( event );
	//	d->tool->init(this);
	//}
}

void KTPaintArea::projectResponse(KTProjectResponse *)
{
}

void KTPaintArea::libraryResponse(KTLibraryResponse *request)
{
	switch(request->action())
	{
		case KTProjectRequest::AddSymbolToProject:
		{
			graphicsScene()->drawCurrentPhotogram();
			viewport()->update(scene()->sceneRect().toRect() );
		}
		break;
	}
	
}

bool KTPaintArea::canPaint() const
{
	if ( ! scene() ) return false;
	
	if ( KTGraphicsScene *sscene = graphicsScene() )
	{
		if (sscene->currentFrameIndex() >= 0 && sscene->currentLayerIndex() >= 0) return true;
	}
	
	return false;
}

void KTPaintArea::deleteItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems ();
	
	if(!selecteds.empty())
	{
		QString strItems= "";
		
		KTGraphicsScene* currentScene = graphicsScene();
		
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				
				KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->indexOf(item), KTProjectRequest::Remove );
				emit requestTriggered(&event);
			}
		}
	}
}

void KTPaintArea::groupItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		QString strItems= "";
		
		KTGraphicsScene* currentScene = graphicsScene();
		int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				if(currentScene->currentFrame()->indexOf(item) != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->indexOf(item)) ;
						firstItem = currentScene->currentFrame()->indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		if(strItems != ")")
		{
			KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Group, strItems );
			emit requestTriggered(&event);
		}
	}
}

void KTPaintArea::ungroupItems()
{
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
// 		QString strItems= "";
		
		KTGraphicsScene* currentScene = graphicsScene();
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->indexOf(item), KTProjectRequest::Ungroup);
				emit requestTriggered(&event);
			}
		}
		/*int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				if(currentScene->currentFrame()->indexOf(item) != -1 && qgraphicsited->cast<QGraphicsItemGroup *>(item) )
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->indexOf(item)) ;
						firstItem = currentScene->currentFrame()->indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		if(strItems != ")")
		{
			KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Ungroup, strItems );
			emit requestTriggered(&event);
	}*/
	}
}

void KTPaintArea::copyItems()
{
	D_FUNCINFO;
	d->copiesXml.clear();
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		KTGraphicsScene* currentScene = graphicsScene();
// 		int firstItem = -1;
		
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				QDomDocument orig;
				orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
				d->copiesXml << orig.toString();
				
				// Paint it to clipbard
				QPixmap toPixmap(item->boundingRect().size().toSize());
				toPixmap.fill(Qt::transparent);
				
				QPainter painter(&toPixmap);
				painter.setRenderHint(QPainter::Antialiasing);
				
				QStyleOptionGraphicsItem opt;
				
				opt.state = QStyle::State_None;
				
				if (item->isEnabled())
					opt.state |= QStyle::State_Enabled;
				if (item->hasFocus())
					opt.state |= QStyle::State_HasFocus;
				if (item == currentScene->mouseGrabberItem())
					opt.state |= QStyle::State_Sunken;
				
				opt.exposedRect = item->boundingRect();
				opt.levelOfDetail = 1;
				opt.matrix = item->sceneMatrix();
				opt.palette = palette();
				
				item->paint(&painter, &opt, this);
				
				painter.end();
				
				QApplication::clipboard()->setPixmap(toPixmap);
			}
		}
	}
	
}

void KTPaintArea::pasteItems()
{
	D_FUNCINFO;
	KTGraphicsScene* currentScene = graphicsScene();
	
	foreach(QString xml, d->copiesXml)
	{
		KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->graphics().count(), KTProjectRequest::Add, xml);
		
		emit requestTriggered(&event);
	}
}

void KTPaintArea::cutItems()
{
	D_FUNCINFO;
	copyItems();
	deleteItems();
}

void KTPaintArea::setNextFramesOnionSkinCount(int n)
{
	if ( KTGraphicsScene* currentScene = graphicsScene() )
	{
		currentScene->setNextOnionSkinCount( n );
	}
}

void KTPaintArea::setPreviousFramesOnionSkinCount(int n)
{
	if ( KTGraphicsScene* currentScene = graphicsScene() )
	{
		currentScene->setPreviousOnionSkinCount( n );
	}
}

void KTPaintArea::addSelectedItemsToLibrary()
{
	dDebug("paintarea") << "Adding to library";
	
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	
	if ( selecteds.isEmpty() )
	{
		DOsd::self()->display(tr("No items selected"), DOsd::Error);
		return;
	}
	
	LibraryDialog dialog;
	foreach (QGraphicsItem *item, selecteds )
	{
		dialog.addItem( item );
	}
	
	if ( dialog.exec() != QDialog::Accepted )
	{
		return;
	}
	
	foreach (QGraphicsItem *item, selecteds )
	{
		if( KTAbstractSerializable *itemSerializable = dynamic_cast<KTAbstractSerializable *>(item) )
		{
			QString symName = dialog.symbolName( item );
			
			QDomDocument doc;
			doc.appendChild(itemSerializable->toXml(doc));
			
			KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,  doc.toString().toLocal8Bit(), KTLibraryObject::Item);
			
			emit requestTriggered(&request);
		}
	}
}


