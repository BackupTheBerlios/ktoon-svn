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

#include "kttoolplugin.h"

#include <dcore/ddebug.h>

KTToolPlugin::KTToolPlugin(QObject * parent) : QObject(parent)
{
}


KTToolPlugin::~KTToolPlugin()
{
}

void KTToolPlugin::init(QGraphicsView *view)
{
	Q_UNUSED(view);
}

void KTToolPlugin::setCurrentTool(const QString &tool)
{
	m_currentTool = tool;
}

QString KTToolPlugin::currentTool() const
{
	return m_currentTool;
}

void KTToolPlugin::begin()
{
	dDebug("tools") << "Begin: " << m_currentTool;
	qDeleteAll(m_events);
	m_events.clear();
	
}

void KTToolPlugin::end()
{
	dDebug("tools") << "End: " << m_currentTool;
}

void KTToolPlugin::addProjectEvent(KTProjectRequest *e)
{
	m_events << e;
}

void KTToolPlugin::insertProjectEvent(int index, KTProjectRequest *e)
{
	if ( index <= m_events.count() )
	{
		m_events.insert(index, e);
	}
	else
	{
		dWarning() << "Cannot insert event, index: " << index;
	}
}

void KTToolPlugin::removeProjectEvent(KTProjectRequest *e)
{
	m_events.removeAll(e);
}

QList<KTProjectRequest *> KTToolPlugin::events() const
{
	return m_events;
}

void KTToolPlugin::itemRequest(const KTItemRequest *event)
{
	Q_UNUSED(event);
}


void KTToolPlugin::doubleClick(const KTInputDeviceInformation *, KTScene *, QGraphicsView * )
{
}

void KTToolPlugin::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
}

QMenu *KTToolPlugin::menu() const
{
	return 0;
}
