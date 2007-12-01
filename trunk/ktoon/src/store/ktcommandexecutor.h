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

#ifndef KTCOMMANDEXECUTOR_H
#define KTCOMMANDEXECUTOR_H

#include <QObject>

class KTProject;
class KTProjectRequest;
class KTFrameResponse;
class KTItemResponse;
class KTSceneResponse;
class KTLayerResponse;
class KTProjectResponse;
class KTLibraryResponse;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class KTCommandExecutor : public QObject
{
	Q_OBJECT;
	public:
		KTCommandExecutor(KTProject *project);
		~KTCommandExecutor();
		
		bool createScene(KTSceneResponse *response);
		bool createLayer(KTLayerResponse *response);
		bool createFrame(KTFrameResponse *response );
		
		bool createItem(KTItemResponse *response);
		
		
		bool convertItem(KTItemResponse *response);
		bool transformItem(KTItemResponse *response);
		bool setPathItem(KTItemResponse *response);
		
		bool removeScene(KTSceneResponse *response);
		bool removeLayer(KTLayerResponse *response);
		bool removeFrame(KTFrameResponse *response);
		
		bool removeItem(KTItemResponse *response);
		bool moveItem(KTItemResponse *response);
		
		bool groupItems(KTItemResponse *response);
		bool ungroupItems(KTItemResponse *response);
		
		bool moveScene(KTSceneResponse *response);
		bool moveLayer(KTLayerResponse *response);
		bool moveFrame(KTFrameResponse *response);
		
		
		bool lockScene(KTSceneResponse *response);
		bool lockLayer(KTLayerResponse *response);
		bool lockFrame(KTFrameResponse *response);
		
		bool renameScene(KTSceneResponse *response);
		bool renameLayer(KTLayerResponse *response);
		bool renameFrame(KTFrameResponse *response);
		
		void selectScene(KTSceneResponse *response);
		bool selectLayer(KTLayerResponse *response);
		bool selectFrame(KTFrameResponse *response);
		
		bool setFrameVisibility(KTFrameResponse *response);
		bool setLayerVisibility(KTLayerResponse *response);
		bool setSceneVisibility(KTSceneResponse *response);
		
		bool createSymbol(KTLibraryResponse *response);
		bool removeSymbol(KTLibraryResponse *response);
		bool addSymbolToProject(KTLibraryResponse *response);
		bool removeSymbolFromProject(KTLibraryResponse *response);
		
		bool createTweening(KTItemResponse *response);
		
		bool expandFrame(KTFrameResponse *response);
		bool pasteFrame( KTFrameResponse *response);
		
	signals:
		void responsed(KTProjectResponse *response);
		
	private:
		KTProject *m_project;

};

#endif
