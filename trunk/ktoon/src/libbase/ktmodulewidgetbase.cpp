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

#include "ddebug.h"

#include <qobject.h>

KTModuleWidgetBase::KTModuleWidgetBase(QWidget *parent, const char *name) : QWidget(parent), KTAbstractProjectEventHandler()
{
	setObjectName(name);

	m_container = new QVBoxLayout(this);
	
	m_container->setMargin(5);
	m_container->setSpacing(1);
	
	adjustSize();
	hide();
}


KTModuleWidgetBase::~KTModuleWidgetBase()
{
}

void KTModuleWidgetBase::addChild(QWidget* child, Qt::Alignment alignment)
{
	m_childs.append(child);
	
	m_container->invalidate();
	m_container->addWidget(child, 0,alignment);
}

void KTModuleWidgetBase::setCaption(const QString &text)
{
	setWindowTitle(text);
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
	return m_container;
}

bool KTModuleWidgetBase::handleProjectEvent(KTProjectEvent *e)
{
	return handleEvent(e);
}

void KTModuleWidgetBase::frameEvent(KTFrameEvent *frameEvent)
{
	Q_UNUSED(frameEvent);
}

void KTModuleWidgetBase::layerEvent(KTLayerEvent *layerEvent)
{
	Q_UNUSED(layerEvent);
}

void KTModuleWidgetBase::sceneEvent(KTSceneEvent *sceneEvent)
{
	Q_UNUSED(sceneEvent);
}

void KTModuleWidgetBase::projectEvent(KTProjectEvent *projectEvent)
{
	Q_UNUSED(projectEvent);
}

void KTModuleWidgetBase::itemEvent(KTItemEvent *event)
{
	Q_UNUSED(event);
}

