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
 


#ifndef KTGRADIENTMANAGER_H
#define KTGRADIENTMANAGER_H

#include "ktgradientselector.h"
#include "ktgradientviewer.h"
#include "kteditspinbox.h"
#include "ktimagebutton.h"
#include "ccbutton.h"
#include <QFrame>

#include "ktradiobuttongroup.h"



#include "ktxyspinbox.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class SpinControl: public QGroupBox
{
	Q_OBJECT
	public:
		SpinControl(const QWidget *parent)
		{
			QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
			layout->setSizeConstraint(QLayout::SetFixedSize);
			m_title = new QLabel(this);
			layout->addWidget(m_title);
			
			m_radius = new QSpinBox(this);
			connect(m_radius, SIGNAL( valueChanged(int) ), this, SIGNAL(radiusChanged(int)));
			m_radius->setMaximum ( 100 );
			layout->addWidget(m_radius);
			
			m_angle = new QSpinBox(this);
			layout->addWidget(m_angle);
			connect( m_angle, SIGNAL( valueChanged(int) ), this, SIGNAL(angleChanged(int)));
			m_angle->setMaximum ( 360 );
			
		};
		~SpinControl(){};
		
		void setSpin(QGradient::Type type)
		{
			switch(type)
			{
				case QGradient::LinearGradient:
				{
					setVisible(false);
					m_angle->setVisible(false);
					m_radius->setVisible(false);
					m_title->setVisible(false);
					break;
				}
				case  QGradient::RadialGradient:
				{
					show();
					m_radius->show();
					m_angle->hide();
					m_title->show();
					m_title->setText("radius");
					break;
				}
				case  QGradient::ConicalGradient:
				{
					show();
					m_radius->hide();
					m_angle->show();
					m_title->show();
					m_title->setText("angle");
					break;
				}
			}
		};
		
		int angle()
		{
			return m_angle->value();
		};
		int radius()
		{
			return m_radius->value();
		};
		
		void setAngle(int angle)
		{
			m_angle->setValue(angle);
		}
		
		void setRadius(int radius)
		{
			m_radius->setValue(radius);
		}
		
	private:
		QSpinBox *m_angle, *m_radius;
		QLabel *m_title;
		
	signals:
		void angleChanged(int angle);
		void radiusChanged(int radius);
};

/**
 * @class KTGradientCreator
 */
class KTGradientCreator : public QFrame
{
	Q_OBJECT
	public:
		enum KTGradientApply{None=-1, Fill, OutLine, FillAndOutLine };
		KTGradientCreator(QWidget *parent = 0);
		~KTGradientCreator();
		
		void setCurrentColor(const QColor &);
		int gradientType();
		void updateGradient();
		QBrush currentGradient();
		
		KTGradientApply gradientApply();
		
		virtual QSize sizeHint () const;
		
		
	private:
		KTGradientSelector *m_selector;
		KTGradientViewer *m_viewer;
		KTRadioButtonGroup *m_type, *m_spread ;
		KTImageButton *m_fill, *m_outLine;
		QSpinBox *m_radius, *m_angle;
		SpinControl *m_spinControl;
		
	public slots:
		void changeType(int type);
		void changeSpread(int spread);
		void changeGradient( const QGradientStops& );
		void setGradient(const QGradient & gradient);
		void emitGradientChanged();
		
	signals:
		void gradientChanged(const QGradient &);
		void controlArrowAdded();
};

#endif
