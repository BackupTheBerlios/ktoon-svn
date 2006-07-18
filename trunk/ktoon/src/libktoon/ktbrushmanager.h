/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTBRUSHMANAGER_H
#define KTBRUSHMANAGER_H

#include <QObject>
#include <QPen>
#include <QBrush>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTBrushManager : public QObject
{
	Q_OBJECT;
	public:
		KTBrushManager(QObject * parent = 0);
		KTBrushManager(const QPen &pen, const QBrush &brush, QObject * parent = 0);
		~KTBrushManager();
		
		void setPen(const QPen &pen);
		QPen pen() const;
		void setBrush(const QBrush &brush);
		QBrush brush() const;
		
		int penWidth() const;
		QColor penColor() const;
		QBrush penBrush() const;
		
		QBrush brushColor() const;
		
	signals:
		void penChanged(const QPen &pen);
		void brushChanged(const QBrush &brush);
		
	private:
		QPen m_pen;
		QBrush m_brush;
		

};

#endif
