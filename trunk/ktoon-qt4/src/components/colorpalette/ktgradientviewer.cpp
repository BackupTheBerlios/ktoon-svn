/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktgradientviewer.h"
#include <QPainter>
#include "ktdebug.h"

#include "ktgradientadjuster.h"
#include <QMouseEvent>
#include <QRectF>

class KTGradientViewer::ControlPoint
{
	public:
		ControlPoint()
		{
			currentIndex = 0;
			points << QPointF(10,50) << QPointF(60,50);
		}
		~ControlPoint() {}
		QVector<QPointF> points;
		int currentIndex;
		
		void selectPoint(const QPointF &point)
		{
			int rate = 5;
			QPointF p2(point + QPoint(rate/2,rate/2));
			QRectF rect(point - QPointF(rate/2,rate/2) , QSizeF(p2.x(), p2.y()) );
			
			QVector<QPointF>::const_iterator it;
			for (it = points.begin(); it != points.end(); ++it)
			{
				if( rect.contains(*it) )
				{
					currentIndex = points.indexOf(*it);
					break;
				}
			}
		}
		void drawPoints(QPainter *p)
		{
			foreach(QPointF point, points)
			{
				if(point == points[currentIndex])
				{
					p->save();
					p->setPen(QPen(Qt::red, 5, Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin ));
				}
				else
				{
					p->save();
					p->setPen(QPen(Qt::blue, 5, Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin ));
				}
				p->drawPoint(point);
				p->restore();
			}
		}
};

KTGradientViewer::KTGradientViewer(QWidget *parent)
	: QFrame(parent), m_angle(360), m_radius(50)
{

	m_controlPoint = new ControlPoint();
	setMaximumSize(100,100);
	setMinimumSize(100,100);
	m_type = QGradient::LinearGradient;
	m_spread =  QGradient::PadSpread;
	setMidLineWidth(2);
	setLineWidth(2);
	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	m_focal = rect().center();
	m_center = rect().center();
}


KTGradientViewer::~KTGradientViewer()
{
	KTEND;
	delete m_controlPoint;
}

void KTGradientViewer::paintEvent( QPaintEvent* e)
{
	createGradient();
	QPainter p;
	p.begin(this);
	p.setBrush(m_gradient);
	p.drawRect(rect());
	p.setPen(QPen(Qt::blue, 5, Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin ));
	
	
	m_controlPoint->drawPoints(&p);
	
	p.end();
	
	QFrame::paintEvent(e);
}

QSize KTGradientViewer::sizeHint() const
{
	return QSize(100,100);
}

void KTGradientViewer::createGradient()
{
	switch(m_type)
	{
		case  QGradient::LinearGradient:
		{
			m_gradient = QLinearGradient(m_controlPoint->points[0], m_controlPoint->points[1]);
			m_gradient.setStops( m_gradientStops);
			m_gradient.setSpread(m_spread);
			break;
		}
		case QGradient::RadialGradient:
		{
			m_gradient = QRadialGradient(m_controlPoint->points[0], m_radius, m_controlPoint->points[1] );
			m_gradient.setStops( m_gradientStops);
			m_gradient.setSpread(m_spread);
			break;
		}
		case QGradient::ConicalGradient:
		{
			m_gradient = QConicalGradient(m_controlPoint->points[0], m_angle);
			m_gradient.setStops( m_gradientStops);
			m_gradient.setSpread(m_spread);
			break;
		}
		default:
		{
			ktFatal() << "Fatal error, the gradient type doesn't exists!";
		}
	}
}
void KTGradientViewer::changeGradient( const QGradientStops& stops)
{
	m_gradientStops = stops;
	repaint();
}

void KTGradientViewer::changeType(int type)
{
	m_type = QGradient::Type(type);
	repaint();
}


void KTGradientViewer::setSpread(int spread)
{
	m_spread = QGradient::Spread(spread);
	repaint();
}

void KTGradientViewer::changeFocal(const QPointF& focal )
{
	m_focal = focal;
	repaint();
}

QGradient KTGradientViewer::gradient()
{
	return m_gradient;
}

void KTGradientViewer::mousePressEvent(QMouseEvent *e)
{
	m_controlPoint->selectPoint(e->pos());
	
	update();
}

void KTGradientViewer::mouseMoveEvent( QMouseEvent * e )
{
	m_controlPoint->points[m_controlPoint->currentIndex] = e->pos();
	update();
}

void KTGradientViewer::changeAngle(int angle)
{
	m_angle = angle;
	update();
}

void KTGradientViewer::changeRadius(int radius)
{
	m_radius = radius;
	update();
}

