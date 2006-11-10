/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTLAYER_H
#define KTLAYER_H

#include <QObject>
#include "ktkeyframe.h"

#include "ktserializableobject.h"

typedef QList<KTKeyFrame *> Frames;

/**
 * @brief Esta clase representa un layer, los layers estan contenidos en KTDocument y contienen KTKeyFrame 's
 * @author David Cuadrado <krawek@toonka.com>
*/

class Q_DECL_EXPORT KTLayer : public KTSerializableObject
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTLayer(QObject *parent = 0);
		
		/**
		 * Construye el layer con un nombre
		 */
		KTLayer(const QString &layerName, QObject * parent = 0);
		
		/**
		 * Destructor
		 */
		~KTLayer();
		
		/**
		 * Retorna los frames del layer
		 */
		Frames frames();
		
		/**
		 * Pone la lista de frames, esta funcion reemplaza los frames anteriores
		 */
		void setFrames(const Frames &frames);
		
		/**
		 * Crea un frame, si addToEnd es verdadero el frame sera añadido al final, de lo contrario sera añadido despues del frame actual.
		 */
		KTKeyFrame *createFrame(const QString & name = QString::null, bool addToEnd = true );
		
		/**
		 * Retorna el frame actual
		 */
		KTKeyFrame *currentFrame();
		
		/**
		 * Pone el frame actual desde un indice
		 */
		void setCurrentFrame(int index);
		
		/**
		 * Pega un frame desde a un indice
		 */
		void pasteFrame(const int& index, const KTKeyFrame* copy);
		
		void cloneFrame(const int& index, int nClones);
		
		/**
		 * Mueve el frame actual, is up es verdadero lo mueve hacia arriba
		 */
		void moveCurrentFrame( bool up);
		
		/**
		 * Remueve el frame actual
		 */
		void removeCurrentFrame();
		
		/**
		 * Bloquea el frame actual
		 */
		void lockCurrentFrame();
		
		/**
		 * Retorna el indice del frame actual
		 */
		int indexCurrentFrame();
		
		/**
		 * Pone el nombre del layer
		 */
		void setLayerName(const QString &name);
		
		/**
		 * Pone la visibilidad del layer
		 */
		void setVisible(bool isVisible);
		
		/**
		 * Retorna el nombre del layer
		 */
		QString layerName() const;
		
		/**
		 * Retorna verdadero si el layer es visible
		 */
		bool isVisible() const;
		
		/**
		 * Reimplementada de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
	
	signals:
		/**
		 * Este signal se emite cuando un frame es creado
		 */
		void frameCreated( const QString &name, bool toEnd );
		
		/**
		 * Este signal se emite cuando la visibilidad cambia
		 */
		void visibilityChanged(bool );
		
		/**
		 * Este signal se emite cuando el frame actual ha sido movido
		 */
		void frameMoved(bool up);
		
		/**
		 * Este signal se emite cuando el frame actual fue removido
		 */
		void frameRemoved();
		
		/**
		 * Este signal se emite cuando el frame actual fue bloqueado
		 */
		void frameLocked();
		
	private:
		Frames m_frames;
		bool m_isVisible;
		QString m_name;
		
		KTKeyFrame *m_currentFrame;
		
		int m_framesCount;

};

#endif
