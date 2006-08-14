/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef DGRADIENTADJUSTER_H
#define DGRADIENTADJUSTER_H

#include <QObject>
#include <QRadialGradient>
#include <QConicalGradient>
#include <QLinearGradient>
#include <QRect>
#include <QMatrix>

#include "dcore/dglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_CORE_EXPORT DGradientAdjuster
{
	protected:
		
// 		DGradientAdjuster();
// 		~DGradientAdjuster();
		
	public:
		/**
		 * @if english
		 * Adjust a gradient to rect
		 * @param gradient gradient to adjust
		 * @param rect rect
		 * @elseif spanish
		 * Ajusta un gradiente a un rectangulo
		 * @param gradient gradiente a ajustar
		 * @param rect rectangulo al cual se va ajustar el gradiente
		 * @endif
		*/
		static QGradient adjustGradient(const QGradient *gradient, const QRect &rect );
		/**
		 * @if english
		 * Adjust a linear gradient to rect
		 * @param gradient gradient to adjust
		 * @param rect rect
		 * @elseif spanish
		 * Ajusta un gradiente lineal a un rectangulo
		 * @param gradient gradiente a ajustar
		 * @param rect rectangulo al cual se va ajustar el gradiente
		 * @endif
		 */
		static QLinearGradient adjustGradient(const QLinearGradient &gradient, const QRect &rect);
		/**
		 * @if english
		 * Adjust a radial gradient to rect
		 * @param gradient gradient to adjust
		 * @param rect rect
		 * @elseif spanish
		 * Ajusta un gradiente radial a un rectangulo
		 * @param gradient gradiente a ajustar
		 * @param rect rectangulo al cual se va ajustar el gradiente
		 * @endif
		 */
		static QRadialGradient adjustGradient(const QRadialGradient &gradient, const QRect &rect);
		/**
		 * @if english
		 * Adjust a conical gradient to rect
		 * @param gradient gradient to adjust
		 * @param rect rect
		 * @elseif spanish
		 * Ajusta un gradiente conico a un rectangulo
		 * @param gradient gradiente a ajustar
		 * @param rect rectangulo al cual se va ajustar el gradiente
		 * @endif
		 */
		static QConicalGradient adjustGradient(const QConicalGradient &gradient, const QRect &rect);
		
		/**
		 * @if english
		 * Maps a gradient to transformation matrix
		 * @param gradient
		 * @param matrix
		 * @elseif spanish
		 * Mapea un gradiente a una matrix de transformacion
		 * @param gradient gradiente a transformar
		 * @param matrix matrix de tranformacion
		 * @endif
		 */
		static QGradient mapGradient(const QGradient *gradient, const QMatrix &matrix );
		
		/**
		 * @if english
		 * Maps a linear gradient to transformation matrix
		 * @param gradient
		 * @param matrix
		 * @elseif spanish
		 * Mapea un gradiente linear a una matrix de transformacion
		 * @param gradient gradiente a transformar
		 * @param matrix matrix de tranformacion
		 * @endif
		 */
		static QLinearGradient mapGradient(const QLinearGradient &gradient, const QMatrix &matrix);
		
		/**
		 * @if english
		 * Maps a radial gradient to transformation matrix
		 * @param gradient
		 * @param matrix
		 * @elseif spanish
		 * Mapea un gradiente radial a una matrix de transformacion
		 * @param gradient gradiente a transformar
		 * @param matrix matrix de tranformacion
		 * @endif
		 */
		static QRadialGradient mapGradient(const QRadialGradient &gradient, const QMatrix &matrix);
		
		/**
		 * @if english
		 * Maps a conical gradient to transformation matrix
		 * @param gradient
		 * @param matrix
		 * @elseif spanish
		 * Mapea un gradiente conico a una matrix de transformacion
		 * @param gradient gradiente a transformar
		 * @param matrix matrix de tranformacion
		 * @endif
		 */
		static QConicalGradient mapGradient(const QConicalGradient &gradient, const QMatrix &matrix);
		
		static QGradient flipGradient(const QGradient *gradient, Qt::Orientation o );
		
		
		static QLinearGradient flipGradient(const QLinearGradient &gradient, Qt::Orientation o);
		
		
		static QRadialGradient flipGradient(const QRadialGradient &gradient,Qt::Orientation o);
		
		
		static QConicalGradient flipGradient(const QConicalGradient &gradient, Qt::Orientation o);
		
};

#endif
