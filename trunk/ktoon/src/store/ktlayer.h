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

#include "ktabstractserializable.h"
#include "ktframe.h"

#include <QDomDocument>
#include <QDomElement>

#include "ktinthash.h"

#include "ktglobal_store.h"

typedef KTIntHash<KTFrame *> Frames;

class KTScene;
class KTProject;

/**
 * @brief Esta clase representa un layer, los layers estan contenidos en KTDocument y contienen KTFrame's
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class STORE_EXPORT KTLayer : public QObject, public KTAbstractSerializable
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTLayer(KTScene *parent);
		
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
		 * Pone el nombre del layer
		 */
		void setLayerName(const QString &name);
		
		/**
		 * Bloquea el layer
		 */
		void setLocked(bool isLocked);
		
		/**
		 * Pone la visibilidad del layer
		 */
		void setVisible(bool isVisible);
		
		/**
		 * Retorna el nombre del layer
		 */
		QString layerName() const;
		
		/**
		 * Returna verdadero cuando el layer esta bloqueado
		*/
		bool isLocked() const;
		
		/**
		 * Retorna verdadero si el layer es visible
		 */
		bool isVisible() const;
		
		KTFrame *createFrame(int position, bool loaded = false);
		
		bool removeFrame(int position);
		
		bool moveFrame(int from, int to);
		
		bool expandFrame(int position, int size);
		
		KTFrame *frame(int position) const;
		
		KTScene *scene() const;
		KTProject *project() const;
		
		int logicalIndexOf(KTFrame *frame) const;
		int visualIndexOf(KTFrame *frame) const;
		
		int logicalIndex() const;
		int visualIndex() const;
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc) const;
		
	private:
		struct Private;
		Private *const d;
};

#endif
