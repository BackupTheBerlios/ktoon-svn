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


#ifndef KTPROJECTREQUEST_H
#define KTPROJECTREQUEST_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "ktglobal_store.h"

class KTProjectRequest;
class KTProjectRequest;
class KTProjectRequest;
class KTProjectRequest;

class STORE_EXPORT KTProjectRequestArgument
{
	public:
		KTProjectRequestArgument(const QString &v);
		~KTProjectRequestArgument();
		
		bool toBool();
		int toInt();
		double toReal();
		QString toString();
		
	private:
		QString m_value;
};

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTProjectRequest
{
	public:
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
			
			Group,
			Ungroup,
			
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
			Item,
			Library
		};
		
		KTProjectRequest(Action action, const QVariant &data = 0);
		
		KTProjectRequest(const QString &doc);
		virtual ~KTProjectRequest();
		
		
		Action action() const;
		
		void setPartName(const QString &name);
		QString partName() const;
		
		void setId(int id);
		virtual int id() const;
		virtual bool isValid() const;
		
		virtual KTProjectRequest *clone() const;
		
		QVariant data() const;
		
	private:
		Action m_action;
		QString m_partName;
		QVariant m_data;
		
		int m_id;
};


#endif


