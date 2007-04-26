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
class KTProjectResponse;

class STORE_EXPORT KTProjectRequestArgument
{
	public:
		KTProjectRequestArgument();
		KTProjectRequestArgument(const QString &v);
		~KTProjectRequestArgument();
		
		void operator = (const QString &value);
		
		void setValue(const QString &value);
		
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
			Add = 1,
			/**
			 * Remueve una parte, el tipo de data es nulo
			 */
			Remove = -Add,
			/**
			 * Mueve una parte, el tipo de data es un entero que significa la nueva posición
			 */
			Move = 2,
			/**
			 * Bloquea una parte, el tipo de data es booleano.
			 */
			Lock = 3,
			/**
			 * Renombra una parte, el tipo de data es una cadena de texto.
			 */
			Rename = 4,
			/**
			 * Selecciona una parte, el tipo de data es nulo
			 */
			Select = 5,
			/**
			 * Cambia la visibilidad
			 */
			View = 6,
			
			Group = 7,
			Ungroup = -Group,
			
			// Items
			Transform = 8,
			Convert = 9,
			EditNodes = 10,
			AddSymbolToProject = 11,
			Tweening = 12,
			Update = 13,
			Expand = 14
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
		
		KTProjectRequest(const QString &xml = 0);
		virtual ~KTProjectRequest();
		
		void setId(int id);
		virtual int id() const;
		virtual bool isValid() const;
		
		QString xml() const;
		
		void setExternal(bool b);
		bool isExternal() const;
		
		
		KTProjectRequest &operator = (const KTProjectRequest &other);

	private:
		struct Private;
		Private *const d;
};


#endif


