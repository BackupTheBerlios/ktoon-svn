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

#ifndef KTABSTRACTPROJECTEVENTHANDLER_H
#define KTABSTRACTPROJECTEVENTHANDLER_H

#include <QObject>
#include "ktglobal_store.h"

class KTProjectRequest;
class KTFrameRequest;
class KTLayerRequest;
class KTSceneRequest;
class KTItemRequest;
class KTPaintAreaEvent;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTAbstractProjectRequestHandler
{
	public:
		KTAbstractProjectRequestHandler();
		virtual ~KTAbstractProjectRequestHandler();
		
		virtual bool handleRequest(KTProjectRequest *event);
		
	protected:
		virtual void itemRequest(KTItemRequest *itemRequest) = 0;
		virtual void frameRequest(KTFrameRequest *frameRequest) = 0;
		virtual void layerRequest(KTLayerRequest *layerRequest) = 0;
		virtual void sceneRequest(KTSceneRequest *sceneRequest) = 0;
		virtual void projectRequest(KTProjectRequest *projectRequest) = 0;
};

#endif
