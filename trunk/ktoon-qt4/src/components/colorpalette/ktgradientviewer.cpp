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

KTGradientViewer::KTGradientViewer(QWidget *parent)
 : QFrame(parent)
{

// 	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	m_type = QGradient::LinearGradient;
	
	setMidLineWidth(2);
	setLineWidth(2);
	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
}


KTGradientViewer::~KTGradientViewer()
{
	KTEND;
}

void KTGradientViewer::paintEvent( QPaintEvent* e)
{
	QPainter p(this);
	createGradient();
	
	p.setBrush(m_gradient);
	
	p.drawRect(rect());
	
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
			m_gradient = QLinearGradient(rect().topLeft(), rect().topRight());
			m_gradient.setStops( m_gradientStops);
			break;
		}
		case QGradient::RadialGradient:
		{
			m_gradient = QRadialGradient(rect().center(), rect().topRight().x(), rect().center());
			m_gradient.setStops( m_gradientStops);
			break;
		}
		case QGradient::ConicalGradient:
		{
			m_gradient = QConicalGradient(rect().center(), 450);
			m_gradient.setStops( m_gradientStops);
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

void KTGradientViewer::changeFocal(double dx, double dy)
{
	m_xFocal = dx;
	m_yFocal = dy;
	
	repaint();
}

QGradient KTGradientViewer::gradient()
{
	return m_gradient;
}
