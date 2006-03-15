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
#include "agrid.h"
#include "ddebug.h"

#include <QPainter>


AGrid::AGrid(QObject *parent) : QObject(parent), m_deltaX(10), m_deltaY(10), m_color(Qt::gray)
{
}

AGrid::AGrid(int dx, int dy,QColor color, QObject *parent): QObject(parent), m_deltaX(dx), m_deltaY(dy), m_color(color)
{
}

AGrid::~AGrid()
{
}

QPainterPath AGrid::createGrid(int width, int height)
{
	m_buffer = QImage(width, height, QImage::Format_RGB32);
	m_buffer.fill(qRgb(255, 255, 255));
	
	m_path = QPainterPath();
	for(int i = 10; i <= width-10; i+=m_deltaX)
	{
		drawLine(i, height-10, i, 10 );
	}
	
	for(int i = 10; i <= height-10; i+=m_deltaY)
	{
		drawLine(width-10, i, 10, i);
	}
	
	QPainter painter(&m_buffer);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::gray);
	painter.drawPath(m_path);
	
	return m_path;
}

QImage AGrid::copy(const QRect & r)
{
	if ( r.intersects(m_buffer.rect()) )
	{
		return m_buffer.copy(r);
	}
	
	return QImage();
}

void AGrid::setDeltaX(int delta)
{
	m_deltaX = delta;
}

void AGrid::setDeltaY(int delta)
{
	m_deltaY = delta;
}

void AGrid::setColor(const QColor &color)
{
	m_color = color;
}

void AGrid::drawLine(int x1, int y1, int x2, int y2)
{
	
	m_path.moveTo(x1, y1);
	m_path.lineTo(x2, y2);
}

QPainterPath AGrid::pathGrid()
{
	return m_path;
}

