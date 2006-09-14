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


#ifndef KTPROJECTEVENT_H
#define KTPROJECTEVENT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "ktglobal_store.h"

class KTProjectEvent;
class KTSceneEvent;
class KTLayerEvent;
class KTFrameEvent;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTProjectEvent
{
	public:
		struct PartPosition
		{
			int sceneIndex;
			int layerIndex;
			int frameIndex;
		};
		
		enum Action
		{
			None = 0,
			/**
			 * Añade una parte, el tipo de data es nulo
			 */
			Add,
			/**
			 * Remueve una parte, el tipo de data es nulo
			 */
			Remove,
			/**
			 * Mueve una parte, el tipo de data es un entero que significa la nueva posición
			 */
			Move,
			/**
			 * Bloquea una parte, el tipo de data es booleano.
			 */
			Lock,
			/**
			 * Renombra una parte, el tipo de data es una cadena de texto.
			 */
			Rename,
			/**
			 * Selecciona una parte, el tipo de data es nulo
			 */
			Select,
			/**
			 * Cambia la visibilidad
			 */
			View,
			
			// Items
			Transform,
			Convert,
			EditNodes
		};
		
		enum Part
		{
			Project = 1000,
			Scene,
			Layer,
			Frame,
			Item
		};
		
		KTProjectEvent(Action action, const QVariant &data = 0);
		virtual ~KTProjectEvent();
		
		
		Action action() const;
		
		void setPartName(const QString &name);
		QString partName() const;
		
		virtual int id() const;
		virtual bool isValid() const;
		
		virtual KTProjectEvent *clone() const;
		
		QVariant data() const;
		
	private:
		Action m_action;
		QString m_partName;
		QVariant m_data;
};

Q_DECLARE_METATYPE(KTProjectEvent::PartPosition);


#endif


