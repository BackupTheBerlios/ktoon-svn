/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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


#ifndef KTBRUSHADJUSTER_H
#define KTBRUSHADJUSTER_H

/**
 * @if english
 * Translate me
 * @elseif spanish
 * @short
 * Esta clase ajusta un QBrush, este es debido a que el contenido de un QBrush es estatico, por ejemplo, los gradientes siempre son creados con unos puntos definidos, si deseamos aplicar el gradiente a un cuadrado mas grande debemos ajustar
 * @endif
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

#include "ktgradientadjuster.h"

#include <QBrush>
#include <QMatrix>

class KTBrushAdjuster
{
	public:
		KTBrushAdjuster();
		~KTBrushAdjuster();
	
	public:
		static QBrush adjustBrush(const QBrush *brush, const QRect &rect );
		static QBrush mapBrush(const QBrush *brush, const QMatrix &matrix  );
};

#endif
