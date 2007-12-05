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

#include "kttoolplugin.h"

#include <QGraphicsView>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"
#include "ktprojectresponse.h"
#include "ktgraphicsscene.h"

#include <kcore/kdebug.h>

struct KTToolPlugin::Private
{
	QString currentTool;
};

KTToolPlugin::KTToolPlugin(QObject * parent) : QObject(parent), k(new Private)
{
}

KTToolPlugin::~KTToolPlugin()
{
	delete k;
}

void KTToolPlugin::init(KTGraphicsScene *scene)
{
	Q_UNUSED(scene);
}

void KTToolPlugin::updateScene(KTGraphicsScene *scene)
{
	Q_UNUSED(scene);
}

void KTToolPlugin::setCurrentTool(const QString &tool)
{
	k->currentTool = tool;
}

QString KTToolPlugin::currentTool() const
{
	return k->currentTool;
}

void KTToolPlugin::begin()
{
	kDebug("tools") << "Begin: " << k->currentTool;
}

void KTToolPlugin::end()
{
	kDebug("tools") << "End: " << k->currentTool;
}

void KTToolPlugin::itemResponse( const KTItemResponse *event)
{
	Q_UNUSED(event);
}

void KTToolPlugin::doubleClick(const KTInputDeviceInformation *, KTGraphicsScene * )
{
}

void KTToolPlugin::aboutToChangeScene(KTGraphicsScene *scene)
{
	Q_UNUSED(scene);
}

void KTToolPlugin::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
}

QMenu *KTToolPlugin::menu() const
{
	return 0;
}
