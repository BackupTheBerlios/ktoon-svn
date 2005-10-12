/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#ifndef KTDISPLAYBRUSH_H
#define KTDISPLAYBRUSH_H

#include <QWidget>
#include <QPixmap>

/**
* @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTDisplayBrush : public QWidget
{
Q_OBJECT
	public:
		KTDisplayBrush(int minThickness, int maxThickness,QWidget *parent = 0);
		~KTDisplayBrush();
		void setForm(QPixmap form);
		
	private:
		int m_minThickness, m_maxThickness;
		QPixmap m_form;
		
	public slots:
		void changeMinThickness(int);
		void changeMaxThickness(int);
		
		
	protected:
		virtual void paintEvent ( QPaintEvent * event );

};

#endif
