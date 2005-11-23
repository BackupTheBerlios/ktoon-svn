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
#ifndef KTBRUSH_H
#define KTBRUSH_H

#include <QObject>
#include <QPainterPath>
#include <QPainter>
#include <QBrush>
#include <QPen>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTBrush : public QObject
{
	public:
		KTBrush();
		KTBrush(const QPainterPath &);
		~KTBrush();
		QPainterPath brushForm() const;
		void setBrushForm(const QPainterPath &);
		
		void setBrush(const QBrush &brush);
		void setPen(const QPen &pen);
		
		void setPenBrush(const QBrush &brush);
		void setPenWidth(double width);
		
		void setupPainter(QPainter *painter);
		
	private:
		void setup();
		
	private:
		QPainterPath m_brushForm;
		int m_thickness;
		QString m_brushName; // TODO: save me!! ;)
		
		QBrush m_brush;
		QPen m_pen;
};

#endif
