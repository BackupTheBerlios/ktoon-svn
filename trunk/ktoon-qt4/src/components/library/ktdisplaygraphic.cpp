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

#include "ktdisplaygraphic.h"

#include "ktdebug.h"
#include "ktgradientadjuster.h"

#include <QPainter>

#include "ktpathadjuster.h"

KTDisplayGraphic::KTDisplayGraphic(QWidget *parent) : QFrame(parent), m_drawGraphic(false), m_graphic(0)
{
	m_displayArea = QImage(100, 100, QImage::Format_RGB32);
	m_displayArea.fill(qRgb(255, 255, 255));
	
	setMidLineWidth(2);
	setLineWidth(2);
	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	
	QPalette pal = palette();
	setPalette(pal);
}


KTDisplayGraphic::~KTDisplayGraphic()
{
	if(m_graphic) delete m_graphic; 
}

QSize KTDisplayGraphic::sizeHint() const
{
	return (QSize(parentWidget()->width(), m_displayArea.height() + 15));
}

void KTDisplayGraphic::paintEvent ( QPaintEvent *e )
{
	QFrame::paintEvent(e);
	
	QPainter painter;
	
	m_displayArea.fill(qRgb(255, 255, 255));
	
	painter.begin(&m_displayArea);
	painter.setRenderHint(QPainter::Antialiasing);
	
	if ( m_drawGraphic )
	{
		// AQUI SE DIBUJA EL COMPONENTE
		drawComponent(m_graphic, &painter);
	}
	
	painter.end();
	
	painter.begin(this);
	
	painter.translate(QPoint(width()/2-50, (height() - m_displayArea.height())/2 ));
	
	painter.drawImage(QPoint(0, 0), m_displayArea);
	
	painter.drawRect(m_displayArea.rect());
}

void KTDisplayGraphic::addGraphicComponent(const AGraphicComponent *component)
{
	AGraphicComponent *newGraphic = new AGraphicComponent(*component);
	newGraphic->adjustToRect(m_displayArea.rect(), 10);
	// AJUSTAR AL RECT
	m_graphic = newGraphic;
	
	m_drawGraphic = true;
	
	update();
}



void KTDisplayGraphic::removeGraphic()
{
	m_drawGraphic = false;
	update();
}



void KTDisplayGraphic::drawComponent(AGraphicComponent *component, QPainter * painter)
{
	
	foreach(AGraphic * graphic, component->graphics())
	{
		painter->save();
		painter->setPen(graphic->pen);
		painter->setBrush(graphic->brush);
		painter->drawPath(graphic->path);
		painter->restore();
	}
	
	if(component->hasChilds())
	{
		foreach(AGraphicComponent * child, component->childs())
		{
			drawComponent(child, painter);
		}
	}
}
