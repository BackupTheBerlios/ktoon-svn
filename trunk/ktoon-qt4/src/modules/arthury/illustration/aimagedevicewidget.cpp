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

AImageDeviceWidget::AImageDeviceWidget(const QSize &size, QWidget *parent) : QWidget(parent)
{
	
	setAttribute(Qt::WA_StaticContents);
// 	setAttribute(Qt::WA_NoBackground);
	device = new QImage(size, QImage::Format_RGB32);
	device->fill(qRgb(255, 255, 255));
	int c= 0;
	adjustSize();
	
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
	Q_UNUSED(e);
	QPainter painter(this);
	painter.drawImage(QPoint(0, 0), *device);
}



