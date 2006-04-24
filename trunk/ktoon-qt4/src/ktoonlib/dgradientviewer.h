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
 
#ifndef DGRADIENTVIEWER_H
#define DGRADIENTVIEWER_H

#include <QFrame>
#include <QGradient>
#include <QList>

/**
 * @if english
 * @short translate me
 * @elseif spanish
 * @short Esta clase provee de una visualizardor de gradientes, ademas de permite modificar:
 * @n-Si es Lineal: el punto inical y punto final 
 * @n-Si es Radial: el centro, punto focal 
 * @n-Si es Conico: el centro,
 * @endif
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class DGradientViewer : public QFrame
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Constructor por defecto.
		 * @endif
		 */
		DGradientViewer(QWidget *parent = 0);
		/**
		 * Destructor
		 */
		~DGradientViewer();
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Devuelve el gradiente actualmente visualizado.
		 * @endif
		 */
		QGradient gradient();
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Crea el gradiente a visualizar con la informacion que tenga de este.
		 * @endif
		 */
		void createGradient();
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * pone el "spread" del gradiente a vidualizar
		 * @endif
		 * @see QGradient
		 */
		void setSpread(int spread);

		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Cambia el gradiente a visualizar.
		 * @endif
		 * @see QGradient
		 */
		void setGradient(const QGradient* gradient);
		
	protected:
		virtual void paintEvent( QPaintEvent * );
		virtual void mousePressEvent(QMouseEvent *e);
		virtual void mouseMoveEvent( QMouseEvent * e );
		virtual QSize sizeHint() const;
		
	private:
		class ControlPoint;
		ControlPoint *m_controlPoint;
		QGradientStops m_gradientStops;
		QGradient m_gradient;
		
		int m_angle, m_radius;
		QGradient::Type m_type;
		QGradient::Spread m_spread;
		
	public slots:
		/**
		* @if english
		* Translate
		* @elseif spanish
		* Cambia los "GradientStops" del actual gradiente.
		* @endif
		* @see QGradient
		 */
		void changeGradientStops( const QGradientStops& );
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Cambia el tipo del acutal gradiente.
		 * @endif
		 * @see QGradient
		 */
		void changeType(int  type);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Cambia el angulo del acutal gradiente, si este es conico.
		 * @endif
		 * @see QGradient
		 */
		void changeAngle(int angle);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Cambia el radio del acutal gradiente, si es radial.
		 * @endif
		 * @see QGradient
		 */
		void changeRadius(int radius);
		
	signals:
		void gradientChanged();
};

#endif
