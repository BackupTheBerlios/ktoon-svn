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

#ifndef KTDOCUMENT_H
#define KTDOCUMENT_H

#include <QObject>
#include "ktscene.h"

#include "ktserializableobject.h"

typedef QList<KTScene *> Scenes;

/**
 * @brief Esta clase es la abstraccion de un documento contenido en KTProjectManager
 * @author David Cuadrado <krawek@toonka.com>
*/

class Q_DECL_EXPORT KTDocument : public KTSerializableObject
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTDocument(QObject *parent = 0);
		
		/**
		 * Destructor
		 */
		~KTDocument();
		
		/**
		 * Retorna la lista de escenas que contiene
		 */
		Scenes scenes() const;
		
		/**
		 * Pone una lista de escenas, esta funcion borra las lista anterior
		 */
		void setScenes(const Scenes &);
		
		/**
		 * Crea una escena, is addToEnd es verdadero añade la escena al final, sino la añade despues de la escena actual.
		 */
		KTScene *createScene(bool addToEnd );
		
		/**
		 * Retorna la escena actual
		 */
		KTScene *currentScene();
		
		/**
		 * Pone la escena actual con un indice
		 */
		void setCurrentScene(int index);
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		/**
		 * Guarda el documento en una ruta
		 */
		void save(const QString &path);
		
		/**
		 * Carga el documento desde una ruta
		 */
		void load(const QString &path);
		
		/**
		 * Pone el nombre del documento
		 */
		void setDocumentName(const QString &name);
		
		
	signals:
		/**
		 * Este signal es emitido cuando se ha creado una escena
		 */
		void sceneCreated(const QString &name, bool toEnd);
		
		/**
		 * Este signal es emitido cuando se cambia una escena
		 */
		void sceneChanged(KTScene *scene);
		
	private:
		Scenes m_scenes;
		KTScene *m_currentScene;
		
		int m_sceneCount;
		mutable QString m_name;
};

#endif
