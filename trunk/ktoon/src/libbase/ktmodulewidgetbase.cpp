/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
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

#include "ktmodulewidgetbase.h"
#include <qtooltip.h>

#include <QEvent>
#include <QBoxLayout>
#include <QDialog>

#include <dcore/ddebug.h>

#include <qobject.h>

struct KTModuleWidgetBase::Private
{
	QBoxLayout *container;
	QObjectList childs;
};

KTModuleWidgetBase::KTModuleWidgetBase(QWidget *parent, const char *name) : QWidget(parent), KTAbstractProjectResponseHandler(), d( new Private )
{
	setObjectName(name);

	d->container = new QVBoxLayout(this);
	
	d->container->setMargin(5);
	d->container->setSpacing(1);
	
	adjustSize();
	hide();
}


KTModuleWidgetBase::~KTModuleWidgetBase()
{
	delete d;
}

void KTModuleWidgetBase::addChild(QWidget* child, Qt::Alignment alignment)
{
	d->childs.append(child);
	
	d->container->invalidate();
	d->container->addWidget(child, 0,alignment);
}

void KTModuleWidgetBase::setFont( const QFont &f)
{
	QWidget::setFont(f);
	adjustSize();
}

bool KTModuleWidgetBase::event( QEvent * e )
{
	if ( e->type() == QEvent::Hide )
	{
		emit activate(false);
	}
	else if ( e->type() == QEvent::Show )
	{
		emit activate(true);
	}

	return QWidget::event(e );
}

void KTModuleWidgetBase::enterEvent(QEvent *e)
{
	Q_UNUSED(e);
}

void KTModuleWidgetBase::leaveEvent(QEvent *e)
{
	Q_UNUSED(e);
}

QBoxLayout *KTModuleWidgetBase::boxLayout()
{
	return d->container;
}

bool KTModuleWidgetBase::handleProjectResponse(KTProjectResponse *response)
{
	return handleResponse(response);
}

void KTModuleWidgetBase::frameResponse(KTFrameResponse *frameResponse)
{
	Q_UNUSED(frameResponse);
}

void KTModuleWidgetBase::layerResponse(KTLayerResponse *layerResponse)
{
	Q_UNUSED(layerResponse);
}

void KTModuleWidgetBase::sceneResponse(KTSceneResponse *sceneResponse)
{
	Q_UNUSED(sceneResponse);
}

void KTModuleWidgetBase::projectResponse(KTProjectResponse *projectResponse)
{
	Q_UNUSED(projectResponse);
}

void KTModuleWidgetBase::itemResponse(KTItemResponse *itemResponse)
{
	Q_UNUSED(itemResponse);
}

void KTModuleWidgetBase::libraryResponse(KTLibraryResponse *libraryResponse)
{
	Q_UNUSED(libraryResponse);
}

