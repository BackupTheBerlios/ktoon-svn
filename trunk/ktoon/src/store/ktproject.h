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

#ifndef KTPROJECT_H
#define KTPROJECT_H

#include "ktabstractserializable.h"
#include "ktglobal.h"

#include <QObject>


class KTScene;
class KTLayer;
class KTFrame;
class KTFrameEvent;
class KTProjectEvent;

typedef QList<KTScene *> Scenes;

/**
 * Esta clase maneja el proyecto, además contiene las diferentes escenas que componen todo el proyecto.
 * 
 * @brief Project manager
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTProject : public QObject, public KTAbstractSerializable
{
	Q_OBJECT;
	public:
		KTProject(QObject *parent = 0);
		~KTProject();
		
		void setProjectName(const QString &name);
		
		QString projectName() const;
		
		KTScene *scene(int position);
		
		KTScene *createScene(int position, const QString &xml = QString());
		KTLayer *createLayer(int scene, int position, const QString &xml = QString());
		KTFrame *createFrame(int scene, int layer, int position, const QString &xml = QString());
		
		bool removeScene(int position);
		bool removeLayer(int scene, int position);
		bool removeFrame(int scene, int layer, int position);
		
		void clear();
		
	protected:
		virtual void fromXml(const QString &xml ) {};
		virtual QString toXml() {};
		
	signals:
		void commandExecuted(KTProjectEvent *command );
		
	private:
		Scenes m_scenes;
		QString m_name;
		
		int m_sceneCounter;
};

#endif
