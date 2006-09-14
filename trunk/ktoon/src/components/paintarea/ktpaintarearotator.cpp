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
#include "ktpaintarearotator.h"
#include "ktpaintarea.h"

KTPaintAreaRotator::KTPaintAreaRotator(QObject *parent, KTPaintArea *view) : QObject(parent), m_view(view)
{
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(applyRotation()));
}


KTPaintAreaRotator::~KTPaintAreaRotator()
{
}


void KTPaintAreaRotator::rotateTo(int angle)
{
	m_rotationAngle = angle;
	
	if ( !m_timer.isActive() )
	{
		m_timer.start(50);
	}
}


void KTPaintAreaRotator::applyRotation()
{
	m_view->setRotationAngle(m_rotationAngle);
	m_timer.stop();
}

