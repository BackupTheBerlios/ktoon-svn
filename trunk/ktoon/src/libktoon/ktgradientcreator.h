/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include <kgui/kimagebutton.h>
#include <kgui/kcirclebutton.h>
#include <kgui/kxyspinbox.h>

#include <QFrame>

#include <QComboBox>
#include <QBoxLayout>

#include "ktglobal.h"

class SpinControl;

 /**
  * 
  * @if english
  * @short translate me
  * @elseif spanish
  * @short Esta clase provee de una simple interfaz grafica para crear gradientes.
  * Consta de un visalizador de gradientes,  un configurador de "stops", dos selector de tipos de gradientes (tipo y "spread").
  * 
  * @endif
  *
  * @author Jorge Cuadrado <kuadrosx@toonka.com>
  */
class KTOON_EXPORT KTGradientCreator : public QFrame
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
        KTGradientCreator(QWidget *parent = 0);
        /**
         * Destructor
         */
        ~KTGradientCreator();
        
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Cambia el color del "stop" actual.
         * @endif
         */
        void setCurrentColor(const QColor &);
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Devuelve el tipo del gradiente actual.
         * @endif
         * @see QGradient
         */
        int gradientType();
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Devuelve un QBrush con el gradiente acutal.
         * @endif
         * @see QBrush
         */
        QBrush currentGradient();
        
        /**
         * Devuelve el tamano ideal
         */
        virtual QSize sizeHint () const;
        
    private:
        KTGradientSelector *m_selector;
        KTGradientViewer *m_viewer;
        QComboBox *m_type, *m_spread ;
        QSpinBox *m_radius, *m_angle;
        SpinControl *m_spinControl;
        
    public slots:
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Cambia el tipo del gradiente actual.
         * @endif
         * @see QGradient
         */
        void changeType(int type);
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Cambia el "spread" del gradiente actual.
         * @endif
         * @see QGradient
         */
        void changeSpread(int spread);
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Cambia los "GradientStops" del gradiente actual.
         * @endif
         * @see QGradientStops
         */
        void changeGradientStops( const QGradientStops& );
        /**
         * @if english
         * Translate
         * @elseif spanish
         * Cambia el gradiente actual.
         * @endif
         */
        void setGradient(const QBrush & gradient);
        
    private slots:
        void emitGradientChanged();
        
    signals:
        void gradientChanged(const QBrush &);
        void controlArrowAdded();
};

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */
class KTOON_EXPORT SpinControl: public QGroupBox
{
    Q_OBJECT
    public:
        SpinControl(QWidget *parent = 0) : QGroupBox(parent)
        {
            QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
            layout->setSizeConstraint(QLayout::SetFixedSize);
            m_title = new QLabel(this);
            layout->addWidget(m_title);
            
            m_radius = new QSpinBox(this);
            connect(m_radius, SIGNAL(valueChanged(int)), this, SIGNAL(radiusChanged(int)));
            m_radius->setMaximum(100);
            layout->addWidget(m_radius);
            
            m_angle = new QSpinBox(this);
            layout->addWidget(m_angle);
            connect(m_angle, SIGNAL(valueChanged(int)), this, SIGNAL(angleChanged(int)));
            m_angle->setMaximum(360);
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
                    setVisible(true);
                    m_radius->show();
                    m_angle->hide();
                    m_title->show();
                    m_title->setText(tr("radius"));
                    break;
                }
                case  QGradient::ConicalGradient:
                {
                    setVisible(true);
                    m_radius->hide();
                    m_angle->show();
                    m_title->show();
                    m_title->setText(tr("angle"));
                    break;
                }
                default: break;
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

#endif
