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

#ifndef KTABSTRACTPROJECTRESPONSEHANDLER_H
#define KTABSTRACTPROJECTRESPONSEHANDLER_H

#include <QObject>
#include "ktglobal_store.h"

class KTPaintAreaEvent;
class KTProjectResponse;
class KTItemResponse;
class KTFrameResponse;
class KTSceneResponse;
class KTLayerResponse;
class KTLibraryResponse;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTAbstractProjectResponseHandler
{
    public:
        KTAbstractProjectResponseHandler();
        virtual ~KTAbstractProjectResponseHandler();
        
        virtual bool handleResponse(KTProjectResponse *response);
        
    protected:
        virtual void itemResponse(KTItemResponse *itemResponse) = 0;
        virtual void frameResponse(KTFrameResponse *frameResponse) = 0;
        virtual void layerResponse(KTLayerResponse *layerResponse) = 0;
        virtual void sceneResponse(KTSceneResponse *sceneResponse) = 0;
        virtual void projectResponse(KTProjectResponse *projectResponse) = 0;
        virtual void libraryResponse(KTLibraryResponse *libraryResponse) = 0;
};

#endif
