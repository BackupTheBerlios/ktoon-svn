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

#include "ktdisplaypath.h"

#include "ktdebug.h"
#include "ktgradientadjuster.h"

#include <QPainter>

KTDisplayPath::KTDisplayPath(QWidget *parent) : QFrame(parent)
{
	m_displayArea = QImage(100, 100, QImage::Format_RGB32);
	m_displayArea.fill(qRgb(255, 255, 255));
	
	setMidLineWidth(2);
	setLineWidth(2);
	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	
	QPalette pal = palette();
	setPalette(pal);
}


KTDisplayPath::~KTDisplayPath()
{
}

QSize KTDisplayPath::sizeHint() const
{
	return (QSize(parentWidget()->width(), m_displayArea.height() + 15));
}

void KTDisplayPath::paintEvent ( QPaintEvent *e )
{
	QFrame::paintEvent(e);
	
	QPainter painter;
	
	m_displayArea.fill(qRgb(255, 255, 255));
	
	painter.begin(&m_displayArea);
	painter.setRenderHint(QPainter::Antialiasing);
	
	painter.setPen(m_pen);
	
	if ( m_brush.gradient() )
	{
		m_brush = QBrush(KTGradientAdjuster::adjustGradient(m_brush.gradient(), m_displayArea.rect()));
	}
	
	painter.setBrush(m_brush);
	
	painter.drawPath(m_currentForm);
	painter.end();
	
	painter.begin(this);
	
	painter.translate(QPoint(width()/2-50, (height() - m_displayArea.height())/2 ));
	
	painter.drawImage(QPoint(0, 0), m_displayArea);
	
	painter.drawRect(m_displayArea.rect());
}

void KTDisplayPath::setThickness(int value)
{
	m_thickness = value;
	repaint();
}

void KTDisplayPath::setPath(const QPainterPath &path)
{
	const float offset = 10;
	QRectF br = path.boundingRect();
	QMatrix matrix;
	
	float sx = 1, sy = 1;
	if ( m_displayArea.width() < br.width() )
	{
		sx = static_cast<float>(m_displayArea.width()-offset) / static_cast<float>(br.width());
	}
	if ( m_displayArea.height() < br.height() )
	{
		sy = static_cast<float>(m_displayArea.height()-offset) / static_cast<float>(br.height());
	}
	
	float factor = qMin(sx, sy);
	matrix.scale(factor, factor);
	m_currentForm = matrix.map(path);
	
	matrix.reset();
	
	QPointF pos = m_currentForm.boundingRect().topLeft();
	
	float tx = offset/2-pos.x(), ty = offset/2-pos.y();
	
	matrix.translate(tx, ty);
	m_currentForm = matrix.map(m_currentForm);
	repaint();
}


void KTDisplayPath::setPen(const QPen &pen)
{
	m_pen = pen;
}

void KTDisplayPath::setBrush(const QBrush &brush)
{
	m_brush = brush;
}

