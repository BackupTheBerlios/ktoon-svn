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

#ifndef KTSCENE_H
#define KTSCENE_H

#include <QObject>
#include "ktlayer.h"

typedef QList<KTLayer *> Layers;

#include "ktserializableobject.h"

/**
 * @brief Esta clase representa una escena
 * @author David Cuadrado <krawek@toonka.com>
*/

class Q_DECL_EXPORT KTScene : public KTSerializableObject
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTScene(QObject *parent = 0);
		
		/**
		 * Destructor
		 */
		~KTScene();
		
		/**
		 * Pone un nombre a la escena
		 */
		void setSceneName(const QString &name);
		
		/**
		 * retorna el nombre de la escena
		 */
		QString sceneName() const;
		
		/**
		 * Retorna la lista de layers
		 */
		Layers layers() const;
		
		/**
		 * Pone la lista de layers, esta funcion sobreescribe los layers anteriores
		 */
		void setLayers(const Layers &);
		
		/**
		 * Retorna el layer actual
		 */
		KTLayer *currentLayer();
		
		/**
		 * Pone el layer actual desde un indice
		 */
		void setCurrentLayer(int index);
		
		/**
		 * Retorna el indice layer actual
		 */
		int indexCurrentLayer() const;
		
		/**
		 * Remueve el layer situado en el indice proporcionado
		 */
		void removeLayer(int index);
		
		/**
		 * Crea una layer, si addToEnd es verdadero el layer se creara al final, sino se creara despues del layer actual
		 */
		KTLayer *createLayer(const QString & name = QString::null, bool addToEnd = true );
		
		/**
		 * Cambia los frames por segundo de la escena
		 */
		void setFPS(int fps);
		
		/**
		 * Retorna los frames por segundo de la escena
		 */
		int fps() const;
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		/**
		 * Guarda la escena en una ruta
		 */
		void save(const QString &scenePath);
		
		/**
		 * Carga la escena desde una ruta
		 */
		void load(const QString &path);
		
		/**
		 * Mueve el current layer, si up es verdadero lo mueve hacia arriba
		 */
		void moveCurrentLayer(bool up);
		
	private slots:
		/**
		 * Carga un layer
		 */
		void loadLayer(const QString &);
		
		/**
		 * Carga un frame
		 */
		void loadFrame(const QString &name, int clones);
		
		/**
		 * Carga un componente
		 */
		void loadComponent(AGraphicComponent *);
		
	signals:
		/**
		 * Este signal es emitido cuando se crea un layer
		 */
		void layerCreated(const QString &name, bool toEnd);
		
		/**
		 * Este signal se emite cuando se remueve un layer
		 */
		void layerRemoved(int index);
		
		/**
		 * Este signal se emite cuando se selecciona un layer
		 */
		void layerSelected(int index);
		
		/**
		 * Este signal se emite cuando el layer ha sido movido, si up es verdadero el layer se movio hacia arriba
		 */
		void layerMoved(bool up);
		
		
	private:
		Layers m_layers;
		KTLayer *m_currentLayer;
		mutable QString m_name;
		
		int m_layerCount;
		
		int m_fps;
};

#endif
