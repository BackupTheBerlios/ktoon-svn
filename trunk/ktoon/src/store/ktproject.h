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
#include <QDomDocument>
#include <QDomElement>
#include "ktglobal_store.h"


class KTScene;
class KTLayer;
class KTFrame;
class KTFrameEvent;
class KTProjectEvent;
class QGraphicsItem;

typedef QList<KTScene *> Scenes;


/**
 * Esta clase maneja el proyecto, además contiene las diferentes escenas que componen todo el proyecto.
 * 
 * @brief Project manager
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class STORE_EXPORT KTProject : public QObject, public KTAbstractSerializable
{
	Q_OBJECT;
	public:
		KTProject(QObject *parent = 0);
		~KTProject();
		
		void setProjectName(const QString &name);
		
		QString projectName() const;
		
		KTScene *scene(int position);
		int indexOf(KTScene *scene) const;
		
		KTScene *createScene(int position, const QString &xml = QString());
		KTLayer *createLayer(int scene, int position, const QString &xml = QString());
		KTFrame *createFrame(int scene, int layer, int position, const QString &xml = QString());
		QGraphicsItem *createItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString transformItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString convertItem(int scenePosition, int layerPosition, int framePosition, int position, const QString &xml);
		QString setPathItem( int scenePosition, int layerPosition, int framePosition, int position, const QString &path );
		
		
		QString removeScene(int position);
		QString removeLayer(int scene, int position);
		QString removeFrame(int scene, int layer, int position);
		void removeItem(int scenePosition, int layerPosition, int framePosition, int position);
		
		void moveScene(int position, int newPosition);
		void moveLayer(int scene, int position, int newPosition);
		void moveFrame(int scene, int layer, int position, int newPosition);
		
		void lockScene(int position, bool lock);
		void lockLayer(int scene, int position, bool lock);
		void lockFrame(int scene, int layer, int position, bool lock);
		
		QString renameScene(int position, const QString &newName);
		QString renameLayer(int scene, int position, const QString &newName);
		QString renameFrame(int scene, int layer, int position, const QString &newName);
		
		void selectScene(int position, bool prioritary);
		void selectLayer(int scene, int position, bool prioritary);
		void selectFrame(int scene, int layer, int position, bool prioritary);
		
		void setFrameVisibility(int scenePos, int layerPos, int position, bool view);
		void setLayerVisibility(int scenePos, int position, bool view);
		void setSceneVisibility(int position, bool view);
		
		
		void clear();
		
		void reemitEvent(KTProjectEvent *event);
		
	protected:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc);
		
	signals:
		void commandExecuted(KTProjectEvent *command );
		
	private:
		Scenes m_scenes;
		QString m_name;
		
		int m_sceneCounter;
};

#endif
