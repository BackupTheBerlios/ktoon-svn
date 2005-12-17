/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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
#ifndef AGRID_H
#define AGRID_H

#include <QObject>
#include <QColor>
#include <QImage>
#include <QPainterPath>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class AGrid : public QObject
{
	Q_OBJECT
	public:
		AGrid(QObject *parent = 0);
		AGrid(int dx, int dy,QColor color, QObject *parent = 0);
		
		~AGrid();
		
		QPainterPath createGrid(int width, int height);
		
		void setDeltaX(int delta);
		void setDeltaY(int delta);
		void setColor(QColor color);
		QPainterPath pathGrid();
		QImage copy(const QRect & r = QRect());
		
	private:
		QImage m_buffer;
		int m_deltaX, m_deltaY;
		QColor m_color;
		void drawLine(int x1, int y1, int x2, int y2);
		QPainterPath m_path;
};

#endif
