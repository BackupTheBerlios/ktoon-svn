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
#include <QPainterPath>

/**
* @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTDisplayBrush : public QWidget
{
Q_OBJECT
	public:
		KTDisplayBrush(QWidget *parent = 0);
		~KTDisplayBrush();
		virtual QSize sizeHint() const;
		
	public slots:
		void setThickness(int value);
		void setForm(const QPainterPath &form);
		
	private:
		int m_thickness;
		QPainterPath m_currentForm;
		
	protected:
		virtual void paintEvent ( QPaintEvent * event );

};

#endif
