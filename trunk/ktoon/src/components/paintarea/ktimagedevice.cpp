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

#include "ktimagedevice.h"
#include <QPainter>
#include <QPaintEngine>

KTImageDevice::KTImageDevice(QWidget *parent) : QWidget(parent)
{
	qDebug("KTImageDevice");
	
	m_image = QImage(300,300, QImage::Format_RGB32);
	m_image.fill(Qt::white);
}


KTImageDevice::~KTImageDevice()
{
	qDebug("~KTImageDevice");
}

QPaintEngine *KTImageDevice::paintEngine() const
{
	qDebug("KTImageDevice: paint engine *****************************************");
	return m_image.paintEngine();
}

void KTImageDevice::paintEvent(QPaintEvent *)
{
	qDebug("KTImageDevice: Paint event #####################################################");
	QPainter p(this);
	p.drawImage(0, 0, m_image);
}










