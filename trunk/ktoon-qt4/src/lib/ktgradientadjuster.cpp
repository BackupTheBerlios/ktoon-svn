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

#include "ktgradientadjuster.h"
#include <QMatrix>

KTGradientAdjuster::KTGradientAdjuster()
{
}

KTGradientAdjuster::~KTGradientAdjuster()
{
}

QGradient KTGradientAdjuster::adjustGradient(const QGradient *gradient, const QRect &rect )
{
	switch( gradient->type() )
	{
		case  QGradient::LinearGradient:
		{
			const QLinearGradient *lg = static_cast<const QLinearGradient *>(gradient);
			
			return adjustGradient(*lg, rect);
			break;
		}
		case QGradient::RadialGradient:
		{
			const QRadialGradient *lg = static_cast<const QRadialGradient *>(gradient);
			return adjustGradient(*lg, rect);
			break;
		}
		case QGradient::ConicalGradient:
		{
			const QConicalGradient *lg = static_cast<const QConicalGradient *>(gradient);
			return adjustGradient(*lg, rect);
			break;
		}
	}
	
	return *gradient;
}

QLinearGradient KTGradientAdjuster::adjustGradient(const QLinearGradient &gradient, const QRect &rect)
{
	const float offset = 10;
	
// 	QRectF toScale(gradient.start(), QSizeF(gradient.finalStop().x(), gradient.finalStop().y()) );
	
// 	QMatrix matrix;
// 	float sx = 1, sy = 1;
// 	if ( rect.width() < toScale.width() )
// 	{
// 		sx = static_cast<float>(rect.width()) / static_cast<float>(toScale.width());
// 	}
// 	else
// 	{
// 		
// 	}
// 	
// 	if ( rect.height() < toScale.height() )
// 	{
// 		sy = static_cast<float>(rect.height()) / static_cast<float>(toScale.height());
// 	}
// 	else
// 	{
// 		
// 	}
	
// 	float factor = qMin(sx, sy);
	
// 	matrix.scale(factor, factor);
	
// 	QRectF gradientRect = matrix.mapRect(toScale);
	
// 	gradientRect.translate((rect.width()-gradientRect.width())/2, (rect.height()-gradientRect.height())/2);
	
// 	toScale.translate((rect.width()-toScale.width())/2, (rect.height()-toScale.height())/2);
	
	QLinearGradient newGradient = QLinearGradient(rect.topLeft(), rect.topRight());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QRadialGradient KTGradientAdjuster::adjustGradient(const QRadialGradient &gradient, const QRect &rect)
{
	
	QRadialGradient newGradient = QRadialGradient(rect.center(), (rect.left()-rect.right())/2, rect.center());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QConicalGradient KTGradientAdjuster::adjustGradient(const QConicalGradient &gradient, const QRect &rect)
{
	QConicalGradient newGradient = QConicalGradient(rect.center(), gradient.angle());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

