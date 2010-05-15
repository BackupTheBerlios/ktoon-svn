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

#ifndef KTPROJECTLOADER_H
#define KTPROJECTLOADER_H

#include <QString>

#include "ktlibraryobject.h"

class KTProject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTProjectLoader
{
    public:
    	KTProjectLoader();
    	~KTProjectLoader();
    	
    	static void createItem(int scenePosition, int layerPosition, int framePosition, int itemPosition, const QString &xml, KTProject *project);

    	static void createFrame(int scenePosition, int layerPosition, int framePosition, const QString &name, KTProject *project);
    	static void createLayer(int scenePosition, int layerPosition, const QString &name, KTProject *project);

    	static void createSoundLayer(int scenePosition, int layerPosition, const QString &name, KTProject *project);
    	
    	static void createScene(int scenePosition, const QString &name, KTProject *project);
    	
    	static void createSymbol(KTLibraryObject::Type type, const QString &name, const QByteArray &data, KTProject *project);
};

#endif
