//
// C++ Implementation: tweenertranslate
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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


#include <dgui/daction.h>
#include <dcore/dalgorithm.h>
#include <dcore/dglobal.h>
#include <dcore/ddebug.h>



#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktgraphicalgorithm.h"
#include "ktgraphicsscene.h"
#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"

#include "kttweenerstep.h"

struct TweenerTranslate::Private
{
	QMap<QString, DAction *> actions;
	QPointF first, end;
	Configurator *configurator;
	KTGraphicsScene *scene;
	bool creatingPath, selecting;
};

TweenerTranslate::TweenerTranslate() : KTToolPlugin(), d(new Private)
{
	setupActions();
	d->configurator = 0;
	d->creatingPath = true;
	d->selecting = false;
}


TweenerTranslate::~TweenerTranslate()
{
	delete d;
}

void TweenerTranslate::init(KTGraphicsScene *scene)
{
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
		d->end = input->pos();
	}
}

void TweenerTranslate::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	
}

void TweenerTranslate::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	
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

void TweenerTranslate::aboutToChangeTool()
{
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
	d->creatingPath = true;
	d->selecting = false;
	foreach(QGraphicsView * view, d->scene->views())
	{
		view->setDragMode (QGraphicsView::NoDrag);
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			item->setFlag(QGraphicsItem::ItemIsSelectable, false);
			item->setFlag(QGraphicsItem::ItemIsMovable, false);
		}
	}
}

void TweenerTranslate::setSelect()
{
	d->creatingPath = false;
	d->selecting = true;
	foreach(QGraphicsView * view, d->scene->views())
	{
		view->setDragMode (QGraphicsView::RubberBandDrag);
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			item->setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
		}
	}
}

void TweenerTranslate::applyTweener()
{
	QPainterPath path;
	path.lineTo(d->end);
	
	
	
	QDomDocument doc;
	QDomElement root = doc.createElement("steps");
	doc.appendChild(root);
	
	int count = 0;
	foreach(QPointF pos, path.toFillPolygon())
	{
		KTTweenerStep step(count);
		step.setPosition(pos);
		root.appendChild(step.toXml(doc));
		count++;
	}
	
	foreach(QGraphicsItem *item, d->scene->selectedItems())
	{
		KTProjectRequest request = KTRequestBuilder::createItemRequest(
				d->scene->currentSceneIndex(),
				d->scene->currentLayerIndex(),
				d->scene->currentFrameIndex(),
				d->scene->currentFrame()->indexOf(item),
				KTProjectRequest::Tweening,
				doc.toString() );
		emit requested(&request);
	}
	
	
}

Q_EXPORT_PLUGIN2(kt_tweenertranslate, TweenerTranslate );

