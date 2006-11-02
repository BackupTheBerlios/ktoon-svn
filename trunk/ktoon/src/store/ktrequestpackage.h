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
#ifndef KTREQUESTPACKAGE_H
#define KTREQUESTPACKAGE_H

#include <QDomDocument>

#include "ktprojectrequest.h"
#include "ktabstractprojectrequesthandler.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTRequestPackage : public QDomDocument, public KTAbstractProjectRequestHandler
{
	public:
		KTRequestPackage(KTProjectRequest *request );
		~KTRequestPackage();
		
	private:
		void appendData(QDomElement &element, const QVariant &data);
		
	protected:
		virtual void itemRequest(KTItemRequest *itemRequest);
		virtual void frameRequest(KTFrameRequest *frameRequest);
		virtual void layerRequest(KTLayerRequest *layerRequest);
		virtual void sceneRequest(KTSceneRequest *sceneRequest);
		virtual void projectRequest(KTProjectRequest *projectRequest);
		virtual void libraryRequest(KTProjectRequest *libraryRequest);
};

#endif
