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

#include "dgradientadjuster.h"
#include <QMatrix>

// DGradientAdjuster::DGradientAdjuster()
// {
// }
// 
// DGradientAdjuster::~DGradientAdjuster()
// {
// }

QGradient DGradientAdjuster::adjustGradient(const QGradient *gradient, const QRect &rect )
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

QLinearGradient DGradientAdjuster::adjustGradient(const QLinearGradient &gradient, const QRect &rect)
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

QRadialGradient DGradientAdjuster::adjustGradient(const QRadialGradient &gradient, const QRect &rect)
{
	QPointF center, focal;
	
	center.setX(((gradient.center().x() / 100) *  rect.width()) + rect.x()  );
	center.setY(((gradient.center().y() / 100) *  rect.height()) + rect.y()  );
	
	focal.setX(((gradient.focalPoint().x() / 100) *  rect.width()) + rect.x()  );
	focal.setY(((gradient.focalPoint().y() / 100) *  rect.height()) + rect.y()  );
	int radius = gradient.radius()/100* (qMax(rect.width(), rect.height()));
	QRadialGradient newGradient = QRadialGradient(center, radius,  focal );
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QConicalGradient DGradientAdjuster::adjustGradient(const QConicalGradient &gradient, const QRect &rect)
{
	QPointF center;
	center.setX(((gradient.center().x() / 100) *  rect.width()) + rect.x()  );
	center.setY(((gradient.center().y() / 100) *  rect.height()) + rect.y()  );
	
	QConicalGradient newGradient = QConicalGradient(center, gradient.angle());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QGradient  DGradientAdjuster::mapGradient(const QGradient *gradient, const QMatrix &matrix )
{
	switch( gradient->type() )
	{
		case  QGradient::LinearGradient:
		{
			const QLinearGradient *lg = static_cast<const QLinearGradient *>(gradient);
			
			return mapGradient(*lg, matrix);
			break;
		}
		case QGradient::RadialGradient:
		{
			const QRadialGradient *lg = static_cast<const QRadialGradient *>(gradient);
			return mapGradient(*lg, matrix);
			break;
		}
		case QGradient::ConicalGradient:
		{
			const QConicalGradient *lg = static_cast<const QConicalGradient *>(gradient);
			return mapGradient(*lg, matrix);
			break;
		}
	}
	
	return *gradient;
}

QLinearGradient  DGradientAdjuster::mapGradient(const QLinearGradient &gradient, const QMatrix &matrix)
{
	const float offset = 10;
	
	QPointF start, final;
	
	start = matrix.map(gradient.start());
	final = matrix.map(gradient.finalStop());
	
	QLinearGradient newGradient = QLinearGradient(start, final);
	
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QRadialGradient  DGradientAdjuster::mapGradient(const QRadialGradient &gradient, const QMatrix &matrix)
{
	QPointF center, focal;

	center = matrix.map(gradient.center());
	focal = matrix.map(gradient.focalPoint());
	QRadialGradient newGradient = QRadialGradient(center, gradient.radius(),  focal );
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}
QConicalGradient  DGradientAdjuster::mapGradient(const QConicalGradient &gradient, const QMatrix &matrix)
{
	QPointF center;
// 	center.setX(((gradient.center().x() / 100) *  rect.width()) + rect.x()  );
// 	center.setY(((gradient.center().y() / 100) *  rect.height()) + rect.y()  );
	center = matrix.map(gradient.center());
	QConicalGradient newGradient = QConicalGradient(center, gradient.angle());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QGradient  DGradientAdjuster::flipGradient(const QGradient *gradient, Qt::Orientation o )
{
	switch( gradient->type() )
	{
		case  QGradient::LinearGradient:
		{
			const QLinearGradient *lg = static_cast<const QLinearGradient *>(gradient);
			
			return flipGradient(*lg, o);
			break;
		}
		case QGradient::RadialGradient:
		{
			const QRadialGradient *lg = static_cast<const QRadialGradient *>(gradient);
			return flipGradient(*lg, o);
			break;
		}
		case QGradient::ConicalGradient:
		{
			const QConicalGradient *lg = static_cast<const QConicalGradient *>(gradient);
			return flipGradient(*lg, o);
			break;
		}
	}
	
	return *gradient;
}

QLinearGradient  DGradientAdjuster::flipGradient(const QLinearGradient &gradient, Qt::Orientation o)
{
	
	QPointF start, final;
	
	start = gradient.start();
	final = gradient.finalStop();
	if ( o == Qt::Horizontal)
	{
		start.setY(-start.y());
		final.setY(-final.y());
	}
	else
	{
		start.setX(-start.x());
		final.setX(-final.x());
	}
	
	QLinearGradient newGradient = QLinearGradient(start, final);
	
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QRadialGradient  DGradientAdjuster::flipGradient(const QRadialGradient &gradient, Qt::Orientation o)
{
	QPointF center, focal;

	center = gradient.center();
	focal = gradient.focalPoint();
	
	if ( o == Qt::Horizontal)
	{
		center.setY(-(center).y());
		focal.setY(-(focal).y());
	}
	else
	{
		center.setX(-(center).x());
		focal.setX(-(focal).x());
	}
	
	QRadialGradient newGradient = QRadialGradient(center, gradient.radius(),  focal );
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}

QConicalGradient  DGradientAdjuster::flipGradient(const QConicalGradient &gradient, Qt::Orientation o)
{
	QPointF center;
	center = gradient.center();
	
	if ( o == Qt::Horizontal)
	{
		center.setY(-(center).y());
	}
	else
	{
		center.setX(-(center).x());
	}
	QConicalGradient newGradient = QConicalGradient(center, gradient.angle());
	newGradient.setStops(gradient.stops());
	newGradient.setSpread(gradient.spread());
	return newGradient;
}












