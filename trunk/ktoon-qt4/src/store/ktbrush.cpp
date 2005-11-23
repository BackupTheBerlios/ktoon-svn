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

#include "ktbrush.h"
#include <ktdebug.h>

KTBrush::KTBrush(): QObject()
{
	KTINIT;
	setup();
}

KTBrush::KTBrush(const QPainterPath &brushForm) : QObject(), m_brushForm(brushForm)
{
	KTINIT;
	setup();
}


KTBrush::~KTBrush()
{
	KTEND;
}

void KTBrush::setup()
{
	m_hasGradient = false;
	m_brush = Qt::transparent;
	m_pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}


QPainterPath KTBrush::brushForm() const
{
	return m_brushForm;
}

void KTBrush::setBrushForm(const QPainterPath &form)
{
	m_brushForm = form;
}

void KTBrush::setBrush(const QBrush &brush )
{
	m_brush = brush;
	if ( m_brush.gradient() )
	{
		m_hasGradient = true;
	}
	else
	{
		m_hasGradient = false;
	}
}

void KTBrush::setPen(const QPen &pen)
{
	m_pen = pen;
}

void KTBrush::setPenBrush(const QBrush &brush)
{
	m_pen.setBrush(brush);
}

void KTBrush::setPenWidth(double width)
{
	m_pen.setWidthF(width);
}

void KTBrush::setupPainter(QPainter *p)
{
	p->setRenderHint(QPainter::Antialiasing, true);
	p->setPen(m_pen);
	

	
	p->setBrush(m_brush);
}

