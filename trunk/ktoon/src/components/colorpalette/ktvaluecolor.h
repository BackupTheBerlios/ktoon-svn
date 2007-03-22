/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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

#ifndef KTVALUECOLOR_H
#define KTVALUECOLOR_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <dgui/deditspinbox.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 * @todo: Rename from ValueColor to ColorValue
*/

class KTItemValueColor : public QFrame
{
	Q_OBJECT
	public:
		KTItemValueColor( const QString &text = QString::null, QWidget *parent = 0);
		~KTItemValueColor();
		int  value();
		void setMax(int);
	private:
		QSpinBox *m_value;
		
				
	public slots:
		void setValue ( int val );
		
	signals:
		void valueChanged ( int i );
		void valueChanged ( const QString & text );
};

class KTValueColor : public QFrame
{
	Q_OBJECT
	public:
		KTValueColor(QWidget *parent = 0);
		~KTValueColor();
		int hue();
		int saturation();
		int value();
		int alpha();
		
		
	private:
		KTItemValueColor *m_valueR, *m_valueG, *m_valueB, *m_valueH, *m_valueS, *m_valueV, *m_valueA;
// 		DEditSpinBox *m_valueR, *m_valueG, *m_valueB, *m_valueH, *m_valueS, *m_valueV, *m_valueA;
		QGridLayout *m_layout;
		bool ok;
		
		
	private:
		void setupForm();
		
	public slots:
		void setColor(const QBrush & brush);
		
	private slots:
		void syncValuesRgb(int value);
		
	signals:
		void brushChanged(const QBrush &);
		void hueChanged(int);
		void saturationChanged(int);
		void valueChanged(int);
		
};



#endif
