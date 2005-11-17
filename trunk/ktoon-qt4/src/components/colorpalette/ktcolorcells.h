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
 
#ifndef KTCOLORCELLS_H
#define KTCOLORCELLS_H

#include <QFrame>
#include <QColor>
#include <QPalette>

#include <QGridLayout> // implemtar FlowLayout
#include <QList>
/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTColorItemCells : public QFrame
{
	Q_OBJECT
	public:
		KTColorItemCells(QColor c, QWidget *parent = 0);
		~KTColorItemCells();
		
	private:
		QColor m_color;
		
	protected:
		virtual void mousePressEvent ( QMouseEvent * e );
		virtual void resizeEvent ( QResizeEvent * event );
		virtual QSize sizeHint () const;
	signals:
		void selectColor( const QColor&);
};

class KTColorCells : public QFrame
{
	Q_OBJECT
	public:
		KTColorCells(QWidget *parent = 0);
		~KTColorCells();
		void fillColorsDefault();
		
		
		
	private:
		QList<QColor> m_colors;
		QGridLayout *m_layout;
		int m_columns, m_rows, m_maxColumns, m_maxRows;
		
		
	public slots:
		void addColor(QColor color );
		void addColors(QList<QColor> colors);
		
	signals:
		void changeColor(const QColor &);
		
};

#endif
