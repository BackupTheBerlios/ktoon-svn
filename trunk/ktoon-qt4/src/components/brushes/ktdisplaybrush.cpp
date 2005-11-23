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
 
#include "ktdisplaybrush.h"
#include <QPainter>
#include <QPainterPath>
#include "ktdebug.h"

#include <QVBoxLayout>

KTDisplayBrush::KTDisplayBrush(QWidget *parent) : QWidget(parent), m_thickness(0)
{
}


KTDisplayBrush::~KTDisplayBrush()
{
}

QSize KTDisplayBrush::sizeHint() const
{
	return (QSize(parentWidget()->width(), 100));
}

void KTDisplayBrush::paintEvent ( QPaintEvent * event )
{
	const QPalette palette = this->palette();
	const QPointF formCurrentPos = m_currentForm.currentPosition();
	
	QMatrix matrix;
	
	matrix.translate((width()/2) - formCurrentPos.x() , (height()/2) - formCurrentPos.y() );
	
	m_currentForm = matrix.map(m_currentForm);
	
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	
	p.setPen( palette.color(QPalette::Foreground) );

	p.drawRect(rect().adjusted(5,5,-5,-5));
	
	p.save();
	
	p.setPen( QPen(Qt::black, m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
	p.drawPath(m_currentForm);
	
	p.restore();	
}

void KTDisplayBrush::setThickness(int value)
{
	m_thickness = value;
	repaint();
}

void KTDisplayBrush::setForm(const QPainterPath &path)
{
	QMatrix matrix;
	matrix.scale(0.50, 0.50);
	
	m_currentForm = matrix.map(path);
	repaint();
}


