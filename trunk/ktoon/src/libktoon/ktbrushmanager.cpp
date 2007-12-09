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

#include "ktbrushmanager.h"

struct KTBrushManager::Private
{
	Private() : pen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap)), brush(Qt::transparent) {}
	QPen pen;
	QBrush brush;
};

KTBrushManager::KTBrushManager(QObject * parent) : QObject(parent), k(new Private)
{
}

KTBrushManager::KTBrushManager(const QPen &pen, const QBrush &brush, QObject * parent) : QObject(parent), k(new Private)
{
	k->pen = pen;
	k->brush = brush;
}

KTBrushManager::~KTBrushManager()
{
	delete k;
}

void KTBrushManager::setPen(const QPen &pen)
{
	k->pen = pen;
	emit penChanged( pen );
}

void KTBrushManager::setPenBrush(const QBrush &brush)
{
	k->pen.setBrush(brush);
	emit penChanged( k->pen );
}


QPen KTBrushManager::pen() const
{
	return k->pen;
}

void KTBrushManager::setBrush(const QBrush &brush)
{
	k->brush = brush;
	emit brushChanged( brush );
}

QBrush KTBrushManager::brush() const
{
	return k->brush;
}


int KTBrushManager::penWidth() const
{
	return k->pen.width();
}

QColor KTBrushManager::penColor() const
{
	return k->pen.color();
}

QBrush KTBrushManager::penBrush() const
{
	return k->pen.brush();
}

QBrush KTBrushManager::brushColor() const
{
	return k->brush.color();
}

