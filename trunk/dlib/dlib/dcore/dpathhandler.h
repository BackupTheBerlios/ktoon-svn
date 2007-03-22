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

#ifndef DPATHHANDLER_H
#define DPATHHANDLER_H

#include <QPainterPath>
#include <QChar>
#include <QStringList>
#include "dcore/dglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_CORE_EXPORT DPathHandler
{
	public:
		/**
		 * @if english
		 * Default constructor
		 * @elseif spanish
		 * Constructor por defecto
		 * @endif
		 * @return 
		 */
		DPathHandler();
		
		/**
		 * Destructor
		 * @return 
		 */
		~DPathHandler();
		
		/**
		 * @if english
		 * Adjusts a QPainterPath to rect
		 * @elseif spanish
		 * Ajusta un QPainterPath a un rectangulo
		 * @endif
		 * @param p 
		 * @param rect 
		 * @param offset 
		 * @return 
		 */
		static QPainterPath toRect(const QPainterPath &p, const QRect &rect, float offset = 10.0f);
		
		/**
		 * @if english
		 * Adjust a QPainterPath list to rect
		 * @elseif spanish
		 * Ajusta una lista de QPainterPath a un rectangulo
		 * @endif
		 * @param l 
		 * @param rect 
		 * @param offset 
		 * @return 
		 */
		static QList<QPainterPath> toRect(const QList<QPainterPath> &l, const QRect &rect, float offset = 10.0f);
		
		/**
		 * @if english
		 * Builds a QPainterPath from a QStringList
		 * @elseif spanish
		 * Construye un QPainterPath desde un QStringList
		 * @endif
		 * @param polygonsStr 
		 * @param sep 
		 * @return 
		 */
		static QPainterPath buildPath(const QStringList &polygonsStr, QChar sep);
		
		static QPainterPath buildPath(const QString &svgpath);

};

#endif
