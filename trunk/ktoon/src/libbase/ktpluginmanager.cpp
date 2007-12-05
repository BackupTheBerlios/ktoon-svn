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

#include "ktpluginmanager.h"
#include "ktfilterinterface.h"
#include "kttoolinterface.h"

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#include <QPluginLoader>
#include <QDir>

KTPluginManager *KTPluginManager::s_instance = 0;

KTPluginManager::KTPluginManager(QObject *parent) : QObject(parent)
{
}

KTPluginManager::~KTPluginManager()
{
	unloadPlugins();
}

KTPluginManager *KTPluginManager::instance()
{
	if (!s_instance)
	{
		s_instance = new KTPluginManager;
	}
	
	return s_instance;
}

void KTPluginManager::loadPlugins()
{
	m_filters.clear();
	m_tools.clear();
	
	QDir m_pluginDirectory = QDir(HOME_DIR+"/plugins/");

	foreach(QString fileName, m_pluginDirectory.entryList(QStringList() << "*.so" << "*.dll" << "*.dy",QDir::Files))
	{
		QPluginLoader *loader = new QPluginLoader(m_pluginDirectory.absoluteFilePath(fileName));
		QObject *plugin = qobject_cast<QObject*>(loader->instance());
		
		kDebug("plugins") << "****** Trying to load plugin from: " << fileName;
		
		if (plugin)
		{
			AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(plugin);
			KTToolInterface *aTool = qobject_cast<KTToolInterface *>(plugin);
			
			if ( aFilter )
			{
				m_filters << plugin;
			}
			else if (aTool)
			{
				m_tools << plugin;
			}
			
			m_loaders << loader;
		}
		else
		{
			kFatal("plugins") << "Cannot load plugin, error was: " << loader->errorString();
		}
	}
}

void KTPluginManager::unloadPlugins()
{
	kDebug("plugins") << "Unloading plugins...";
	foreach(QPluginLoader *loader, m_loaders)
	{
		delete loader->instance();
		delete loader;
	}
}


QObjectList KTPluginManager::tools() const
{
	return m_tools;
}

QObjectList KTPluginManager::filters() const
{
	return m_filters;
}






