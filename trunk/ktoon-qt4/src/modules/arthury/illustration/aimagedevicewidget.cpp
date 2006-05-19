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

#include "aimagedevicewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include "ddebug.h"

AImageDeviceWidget::AImageDeviceWidget(const QSize &size, QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);
	setAttribute(Qt::WA_StaticContents);
// 	setAttribute(Qt::WA_NoBackground);
	device = new QImage(size, QImage::Format_RGB32);
	device->fill(m_fillColor.rgb());
	int c= 0;
	adjustSize();
	
	m_fillColor = Qt::white;
}

AImageDeviceWidget::~AImageDeviceWidget()
{
	if ( device ) delete device;
}

QSize AImageDeviceWidget::sizeHint() const
{
	return device->size();
}

void AImageDeviceWidget::paintEvent(QPaintEvent *e)
{	
	QRect rupdate = e->rect ();
	
	QPainter painter(this);
	painter.setClipRect(rupdate);
	painter.drawImage(QPoint(0, 0), *device);
}

void AImageDeviceWidget::resizeEvent ( QResizeEvent * event )
{
	device = new QImage(event->size(), QImage::Format_RGB32);
	device->fill(m_fillColor.rgb());
}

void AImageDeviceWidget::mouseMoveEvent(QMouseEvent *e)
{
	emit mousePos( mapToParent(e->pos()) );
	
	QWidget::mouseMoveEvent(e);
}


void AImageDeviceWidget::setBackgroundColor(const QColor &color)
{
	m_fillColor = color;
	device->fill(color.rgb());
}

