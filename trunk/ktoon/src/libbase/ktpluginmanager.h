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

#ifndef KTPLUGINMANAGER_H
#define KTPLUGINMANAGER_H

#include <QObject>
#include "ktglobal.h"

class QPluginLoader;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT KTPluginManager : public QObject
{
	Q_OBJECT
	private:
		KTPluginManager(QObject *parent = 0);
		
	public:
		~KTPluginManager();
		
	public:
		static KTPluginManager *instance();
		static KTPluginManager *s_instance;
		
		void loadPlugins();
		
		QObjectList tools() const;
		QObjectList filters() const;
		
	private:
		QObjectList m_tools;
		QObjectList m_filters;
		
		QList<QPluginLoader *> m_loaders;
};

#endif
