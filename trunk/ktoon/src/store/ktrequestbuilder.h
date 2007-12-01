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

#ifndef KTREQUESTBUILDER_H
#define KTREQUESTBUILDER_H

#include <QString>
#include <QDomDocument>
#include <QVariant>

#include "ktlibraryobject.h"

#include "ktglobal_store.h"

class KTProjectRequest;
class KTProjectResponse;
/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTRequestBuilder
{
	protected:
		KTRequestBuilder();
		
	public:
		~KTRequestBuilder();
		
		static KTProjectRequest createItemRequest(int sceneIndex, int layerIndex, int frameIndex, int itemIndex, int action, const QVariant &arg = QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createFrameRequest(int sceneIndex, int layerIndex, int frameIndex, int action, const QVariant &arg= QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createLayerRequest(int sceneIndex, int layerIndex, int action, const QVariant &arg= QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createSceneRequest(int sceneIndex, int action, const QVariant &arg= QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createLibraryRequest(int actionId, const QVariant &arg, KTLibraryObject::Type type, const QByteArray &data, int scene = -1, int layer = -1, int frame = -1  );
		
		static KTProjectRequest fromResponse(KTProjectResponse *response);
		
	private:
		static void appendData(QDomDocument &doc, QDomElement &element, const QByteArray &data);
};

#endif
