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
	
	QPointF start, final;
	
	start.setX(((gradient.start().x() / 100) *  rect.width()) + rect.x()  );
	start.setY(((gradient.start().y() / 100) *  rect.height()) + rect.y()  );
	final.setX(((gradient.finalStop().x() / 100) *  rect.width()) + rect.x()  );
	final.setY(((gradient.finalStop().y() / 100) *  rect.height()) + rect.y()  );

			
	QLinearGradient newGradient = QLinearGradient(start, final);
	
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QRadialGradient KTGradientAdjuster::adjustGradient(const QRadialGradient &gradient, const QRect &rect)
{
	QPointF center, focal;
	// 	
	center.setX(((gradient.center().x() / 100) *  rect.width()) + rect.x()  );
	center.setY(((gradient.center().y() / 100) *  rect.height()) + rect.y()  );
	
	focal.setX(((gradient.focalPoint().x() / 100) *  rect.width()) + rect.x()  );
	focal.setY(((gradient.focalPoint().y() / 100) *  rect.height()) + rect.y()  );
	
	QRadialGradient newGradient = QRadialGradient(center, gradient.radius(),  focal );
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QConicalGradient KTGradientAdjuster::adjustGradient(const QConicalGradient &gradient, const QRect &rect)
{
	QPointF center;
	center.setX(((gradient.center().x() / 100) *  rect.width()) + rect.x()  );
	center.setY(((gradient.center().y() / 100) *  rect.height()) + rect.y()  );
	
	QConicalGradient newGradient = QConicalGradient(center, gradient.angle());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

