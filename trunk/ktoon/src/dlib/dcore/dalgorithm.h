/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DALGORITHM_H
#define DALGORITHM_H

#include <QString>
#include <QColor>

#include "dglobal.h"

/**
 * Class that contains generic useful algorithms
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_CORE_EXPORT DAlgorithm
{
	public:
// 		DAlgorithm();
// 		~DAlgorithm();
		/**
		 * @if english
		 * Returns a random integer
		 * @elseif spanish
		 * Retorna una entero aleatorio
		 * @endif
		 * @return 
		 */
		static int random();
		/**
		 * @if english
		 * Returns an random string, this may be useful for example temporaly files
		 * @elseif spanish
		 * Retorna una cadena de caracteres aleatoria, esta funcion puede ser muy util por ejemplo para archivos temporales
		 * @endif
		 * @param length 
		 * @return 
		 */
		static QString randomString(int length);
		
		/**
		 * @if english
		 * Returns a random color
		 * @elseif spanish
		 * Retorna un color aleatorio
		 * @endif
		 * 
		 * @param withAlpha 
		 * @return 
		 */
		static QColor randomColor(bool withAlpha = false);
};

#endif
