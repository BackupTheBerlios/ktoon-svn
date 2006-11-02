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

#ifndef KTCOMMANDEXECUTOR_H
#define KTCOMMANDEXECUTOR_H

#include <QObject>

class KTProject;
class KTProjectRequest;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTCommandExecutor : public QObject
{
	Q_OBJECT;
	public:
		enum State
		{
			None = 0x00,
			Do,
			Redo,
			Undo
		};
		
		KTCommandExecutor(KTProject *project);
		~KTCommandExecutor();
		
		QString createScene(int position, const QString &xml = QString());
		QString createLayer(int scene, int position, const QString &xml = QString());
		QString createFrame(int scene, int layer, int position, const QString &xml = QString());
		QString createItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString transformItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString convertItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString setPathItem( int scenePosition, int layerPosition, int framePosition, int position, const QString &path );
		QString createSymbol(const QString &xml);
		
		
		QString removeScene(int position);
		QString removeLayer(int scene, int position);
		QString removeFrame(int scene, int layer, int position);
		QString removeSymbol(const QString &xml);
		
// 		QString removeItem(int scenePosition, int layerPosition, int framePosition, int position);
		QStringList removeItems(int scenePosition, int layerPosition, int framePosition, int position, const QString &strList );
		
		QStringList groupItems(int scenePosition, int layerPosition, int framePosition, int position, const QString &strList );
		
		QString moveScene(int position, int newPosition);
		QString moveLayer(int scene, int position, int newPosition);
		QString moveFrame(int scene, int layer, int position, int newPosition);
		
		QString lockScene(int position, bool lock);
		QString lockLayer(int scene, int position, bool lock);
		QString lockFrame(int scene, int layer, int position, bool lock);
		
		QString renameScene(int position, const QString &newName);
		QString renameLayer(int scene, int position, const QString &newName);
		QString renameFrame(int scene, int layer, int position, const QString &newName);
		
		QString selectScene(int position, bool prioritary);
		QString selectLayer(int scene, int position, bool prioritary);
		QString selectFrame(int scene, int layer, int position, bool prioritary);
		
		QString setFrameVisibility(int scenePos, int layerPos, int position, bool view);
		QString setLayerVisibility(int scenePos, int position, bool view);
		QString setSceneVisibility(int position, bool view);
		
		
		void reemitEvent(KTProjectRequest *event);
		
		void setState(KTCommandExecutor::State state);
		
	signals:
		void commandExecuted(KTProjectRequest *event, int state);
		
	private:
		KTProject *m_project;
		State m_state;

};

#endif
