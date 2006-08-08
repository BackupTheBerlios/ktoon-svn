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

QString KTToolPlugin::toolToXml() const
{
	return QString();
}

KTProjectEvent::Action KTToolPlugin::action() const
{
	return KTProjectEvent::None;
}