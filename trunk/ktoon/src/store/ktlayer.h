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

typedef QList<KTFrame *> Frames;

/**
 * @brief Esta clase representa un layer, los layers estan contenidos en KTDocument y contienen KTFrame's
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class Q_DECL_EXPORT KTLayer : public QObject, public KTAbstractSerializable
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTLayer(QObject *parent = 0);
		
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
		
		KTFrame *createFrame(int position);
		
		bool removeFrame(int position);
		
		KTFrame *frame(int position);
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc);
		
	private:
		Frames m_frames;
		bool m_isVisible;
		QString m_name;
		int m_framesCount;
};

#endif
