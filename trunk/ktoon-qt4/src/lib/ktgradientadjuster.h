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

#ifndef KTGRADIENTADJUSTER_H
#define KTGRADIENTADJUSTER_H

#include <QObject>
#include <QRadialGradient>
#include <QConicalGradient>
#include <QLinearGradient>
#include <QRect>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTGradientAdjuster
{
	protected:
		KTGradientAdjuster();
		~KTGradientAdjuster();
		
	public:
		static QGradient adjustGradient(const QGradient *gradient, const QRect &rect );
		static QLinearGradient adjustGradient(const QLinearGradient &gradient, const QRect &rect);
		static QRadialGradient adjustGradient(const QRadialGradient &gradient, const QRect &rect);
		static QConicalGradient adjustGradient(const QConicalGradient &gradient, const QRect &rect);
		
		
		static QGradient mapGradient(const QGradient *gradient, const QMatrix &matrix );
		static QLinearGradient mapGradient(const QLinearGradient &gradient, const QMatrix &matrix);
		static QRadialGradient mapGradient(const QRadialGradient &gradient, const QMatrix &matrix);
		static QConicalGradient mapGradient(const QConicalGradient &gradient, const QMatrix &matrix);
		
};

#endif
