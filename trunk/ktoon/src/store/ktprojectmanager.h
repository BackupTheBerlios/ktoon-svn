/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                          	   *
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

#ifndef KTPROJECTMANAGER_H
#define KTPROJECTMANAGER_H

#include <QObject>
#include "ktglobal.h"


class KTSceneManager;
class KTLayer;
class KTFrame;
class KTFrameCommand;
class KTProjectCommand;

typedef QList<KTSceneManager *> SceneManagerList;

/**
 * Esta clase maneja el proyecto, contiene KTSceneManager 's
 * 
 * @brief Project manager
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTProjectManager : public QObject
{
	Q_OBJECT
	public:
		KTProjectManager(QObject *parent = 0);
		~KTProjectManager();
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
// 		QDomElement createXML( QDomSceneManager &doc );
		
		void setProjectName(const QString &name);
		
		QString projectName() const;
		
		bool isOpen();
		void init();
		
		KTSceneManager *createSceneManager(bool addToEnd = true);
		KTSceneManager *currentSceneManager();
		
		KTLayer *createLayer(bool addToEnd = true);
		KTLayer *currentLayer();
		
		KTFrame *createFrame(bool addToEnd = true);
		KTFrame *currentFrame();
		
	public slots:
		/**
		 * Guarda el proyecto
		 */
// 		void save();
		
		void close();
		
		/**
		 * Carga del proyecto desde una ruta
		 */
// 		bool load(const QString &path);
		
	signals:
		void commandExecuted(KTProjectCommand *command );
		
	private:
		SceneManagerList m_sceneManagerList;
		QString m_name;
		bool m_isOpen;
		
		int m_currentSceneManagerIndex;
};

#endif
