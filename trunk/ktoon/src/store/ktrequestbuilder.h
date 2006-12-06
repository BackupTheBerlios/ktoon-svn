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

#ifndef KTREQUESTBUILDER_H
#define KTREQUESTBUILDER_H

#include <QString>
#include <QDomDocument>

class KTProjectRequest;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTRequestBuilder
{
	protected:
		KTRequestBuilder();
		
	public:
		~KTRequestBuilder();
		
		static KTProjectRequest createItemRequest(int sceneIndex, int layerIndex, int frameIndex, int itemIndex, int action, const QString &arg = QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createFrameRequest(int sceneIndex, int layerIndex, int frameIndex, int action, const QString &arg= QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createLayerRequest(int sceneIndex, int layerIndex, int action, const QString &arg= QString(), const QByteArray &data = QByteArray());
		
		static KTProjectRequest createSceneRequest(int sceneIndex, int action, const QString &arg= QString(), const QByteArray &data = QByteArray());
		
	private:
		static void appendData(QDomDocument &doc, QDomElement &element, const QByteArray &data);
};

#endif
