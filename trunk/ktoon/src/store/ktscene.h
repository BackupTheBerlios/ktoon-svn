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

#ifndef KTSCENEMANAGER_H
#define KTSCENEMANAGER_H

#include "ktabstractserializable.h"
#include "ktlayer.h"
#include "ktproject.h"

#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsScene>
#include <QMap>

#include "ktglobal_store.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;

typedef QVector<KTLayer *> Layers;

/**
 * @brief Esta clase representa una escena
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class STORE_EXPORT KTScene : public QObject, public KTAbstractSerializable
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTScene(KTProject *parent);
		
		/**
		 * Destructor
		 */
		~KTScene();
		
		/**
		 * Pone un nombre a la escena
		 */
		void setSceneName(const QString &name);
		
		/**
		 * Bloquea la escena
		 */
		void setLocked(bool isLocked);
		
		/**
		 * retorna el nombre de la escena
		 */
		QString sceneName() const;
		
		/**
		 * Returna verdadero cuando la escena esta bloqueada
		 */
		bool isLocked() const;
		
		
		void setVisible(bool isVisible);
		bool isVisible() const;
		
		/**
		 * Retorna la lista de layers
		 */
		Layers layers() const;
		
		
		KTLayer *layer(int position);
		
		void setLayers(const Layers &);
		
		/**
		 * Remueve el layer situado en el indice proporcionado
		 */
		bool removeLayer(int index);
		
		/**
		 * Crea una layer, si addToEnd es verdadero el layer se creara al final, sino se creara despues del layer actual
		 */
		KTLayer *createLayer(int position, bool loaded = false);
		
		/**
		 * Mueve el layer a la posicicion indicada
		 */
		bool moveLayer(int from, int to);
		
		int index() const;
		int indexOf(KTLayer *layer) const;
		
		KTProject *project() const;
		
		void addTweeningObject(KTGraphicObject *object);
		QList<KTGraphicObject *> tweeningObjects() const;
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc) const;
		
	private:
		struct Private;
		Private *const d;
};

#endif
