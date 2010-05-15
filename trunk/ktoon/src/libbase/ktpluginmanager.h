/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
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
		void unloadPlugins();
		
		QObjectList tools() const;
		QObjectList filters() const;
		
	private:
		QObjectList m_tools;
		QObjectList m_filters;
		
		QList<QPluginLoader *> m_loaders;
};

#endif
