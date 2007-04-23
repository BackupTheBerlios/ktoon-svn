/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "tweenertranslate.h"
#include "configurator.h"

#include <QPointF>
#include <QKeySequence>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMatrix>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QDomDocument>
#include <QGraphicsPathItem>

#include <dgui/daction.h>
#include <dcore/dalgorithm.h>
#include <dcore/dglobal.h>
#include <dcore/ddebug.h>
#include <dgui/dnodegroup.h>



#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktgraphicalgorithm.h"
#include "ktgraphicsscene.h"
#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"
#include "ktscene.h"
#include "ktlayer.h"


struct TweenerTranslate::Private
{
	QMap<QString, DAction *> actions;
	Configurator *configurator;
	KTGraphicsScene *scene;
	bool creatingPath, selecting;
	QGraphicsPathItem *path;
	DNodeGroup *group;
};

TweenerTranslate::TweenerTranslate() : KTToolPlugin(), d(new Private)
{
	setupActions();
	d->configurator = 0;
	d->creatingPath = true;
	d->selecting = false;
	d->path = 0;
	d->group = 0;
}


TweenerTranslate::~TweenerTranslate()
{
	delete d;
}

void TweenerTranslate::init(KTGraphicsScene *scene)
{
	delete d->path;
	d->path = 0;
	delete d->group;
	d->group = 0;
	
	d->scene = scene;
	setCreatePath();
}

QStringList TweenerTranslate::keys() const
{
	return QStringList() << tr("TweenerTranslater");
}

void TweenerTranslate::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	if(d->creatingPath)
	{
		if(!d->path)
		{
			d->path = new QGraphicsPathItem;
			QColor pen = Qt::black;
			pen.setAlpha(125);
			d->path->setPen(QPen(QBrush(pen),1, Qt::DotLine));
			QPainterPath path;
			path.moveTo(input->pos());
			d->path->setPath(path);
			scene->addItem(d->path);
		}
		else
		{
			QPainterPath path = d->path->path();
// 			path.lineTo(input->pos());
			path.cubicTo(input->pos(), input->pos(), input->pos());
			d->path->setPath(path);
		}
	}
}

void TweenerTranslate::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
}

void TweenerTranslate::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	if(d->creatingPath)
	{
		if(!d->group)
		{
			d->group = new DNodeGroup(d->path, scene);
			connect(d->group, SIGNAL(nodeClicked()), SLOT(updatePath()));
			d->group->clearChangesNodes();
		}
		else
		{
			d->group->createNodes(d->path);
		}
		d->configurator->updateSteps(d->path);
	}
}

QMap<QString, DAction *> TweenerTranslate::actions() const
{
	return d->actions;
}

int TweenerTranslate::toolType() const
{
	return KTToolInterface::Brush;
}


QWidget *TweenerTranslate::configurator()
{
	if(!d->configurator)
	{
		d->configurator = new Configurator;
		connect(d->configurator, SIGNAL(clikedCreatePath()), this, SLOT(setCreatePath()));
		connect(d->configurator, SIGNAL(clikedSelect()), this, SLOT(setSelect()));
		connect(d->configurator, SIGNAL(clikedApplyTweener()), this, SLOT(applyTweener()));
	}
	return d->configurator;
}

void TweenerTranslate::aboutToChangeScene(KTGraphicsScene *)
{
}

void TweenerTranslate::aboutToChangeTool()
{
	delete d->group;
	d->group = 0;
	delete d->path;
	d->path = 0;
	setCreatePath();
}

bool TweenerTranslate::isComplete() const
{
	return true;
}

void TweenerTranslate::setupActions()
{
	DAction *translater = new DAction( QIcon(), tr("Tweener Translater"), this);
	d->actions.insert( "TweenerTranslater", translater );
}


void TweenerTranslate::setCreatePath()
{
	if(d->path)
	{
		d->scene->addItem(d->path);
	}
	d->creatingPath = true;
	d->selecting = false;
	foreach(QGraphicsView * view, d->scene->views())
	{
		view->setDragMode (QGraphicsView::NoDrag);
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			if(item != d->path)
			{
				item->setFlag(QGraphicsItem::ItemIsSelectable, false);
			}
		}
	}
}

void TweenerTranslate::setSelect()
{
	d->creatingPath = false;
	d->selecting = true;
	if(d->path)
	{
		delete d->group;
		d->group = 0;
	}
	foreach(QGraphicsView * view, d->scene->views())
	{
		view->setDragMode (QGraphicsView::RubberBandDrag);
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			if(item != d->path)
			{
				item->setFlags(QGraphicsItem::ItemIsSelectable);
			}
		}
	}
}

void TweenerTranslate::applyTweener()
{
	if(d->path)
	{
		foreach(QGraphicsItem *item, d->scene->selectedItems())
		{
			
			KTProjectRequest request = KTRequestBuilder::createItemRequest(
					d->scene->currentSceneIndex(),
					d->scene->currentLayerIndex(),
					d->scene->currentFrameIndex(),
					d->scene->currentFrame()->visualIndexOf(item),
					KTProjectRequest::Tweening, d->configurator->steps()
					);
			emit requested(&request);
			
			if(KTLayer *layer = d->scene->scene()->layer(d->scene->currentLayerIndex()))
			{
				int frames = layer->frames().count();
			
			
				int newFrames = d->configurator->totalSteps() + d->scene->currentFrameIndex() - frames;
			 
				for(int i = frames; i < newFrames; i++)
				{
					KTProjectRequest requestFrame = KTRequestBuilder::createFrameRequest(d->scene->currentSceneIndex(), d->scene->currentLayerIndex(), i, KTProjectRequest::Add);
					emit requested(&requestFrame);
				}
			}
		}
	}
}

void TweenerTranslate::updatePath()
{
	d->configurator->updateSteps(d->path);
}

Q_EXPORT_PLUGIN2(kt_tweenertranslate, TweenerTranslate );

