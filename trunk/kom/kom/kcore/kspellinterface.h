/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KSPELLINTERFACE_H
#define KSPELLINTERFACE_H

#include <QString>
#include <QStringList>
#include "kcore/kglobal.h"

/**
 * @if english
 * Abstract class that define a interface for spell programs, such as aspell, ispell, pspell
 * @elseif spanish
 * Clase abstracta que define una interfaz para programas de ortografia
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KSpellInterface
{
	public:
		virtual ~KSpellInterface() {}
		/**
		 * @if english
		 * This function must check if a word is right
		 * @elseif spanish
		 * Esta funcion debe chequear si una palabra es correcta
		 * @endif
		 * @param word 
		 * @return 
		 */
		virtual bool checkWord(const QString &word) = 0;
		
		/**
		 * @if english
		 * This function must return a list of suggestion
		 * @elseif spanish
		 * Esta funcion debe retorna una lista de sugerencias
		 * @endif
		 * @param word 
		 * @return 
		 */
		virtual QStringList suggestions(const QString &word) = 0;
};

#endif