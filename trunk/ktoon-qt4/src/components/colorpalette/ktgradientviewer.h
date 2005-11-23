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
 
#ifndef KTGRADIENTVIEWER_H
#define KTGRADIENTVIEWER_H

#include <QFrame>
#include <QGradient>

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTGradientViewer : public QFrame
{
	Q_OBJECT
	public:
		KTGradientViewer(QWidget *parent = 0);
		~KTGradientViewer();
		
	protected:
		virtual void paintEvent( QPaintEvent * );
		
	private:
		QGradientStops m_gradientStops;
		QGradient m_gradient;
		QGradient::Type m_type;
		double m_xFocal, m_yFocal;
		
	public slots:
		void changeGradient( const QGradientStops& );
		void changeType(int  type);
		void changeFocal(double dx, double dy);
	
};

#endif
