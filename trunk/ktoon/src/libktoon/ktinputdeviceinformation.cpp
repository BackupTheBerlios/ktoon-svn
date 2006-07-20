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

#include "ktinputdeviceinformation.h"
#include <QMouseEvent>
#include <QTabletEvent>

KTInputDeviceInformation::KTInputDeviceInformation(QObject *parent) : QObject(parent)
{
	m_mouseInfo.button = Qt::NoButton;
	m_mouseInfo.buttons = Qt::NoButton;
	
	m_tabletInfo.pressure = -1;
	m_tabletInfo.rotation = 0;
	m_tabletInfo.tangentialPressure = -1;
	
	m_keyModifiers = Qt::NoModifier;
}


KTInputDeviceInformation::~KTInputDeviceInformation()
{
}

void KTInputDeviceInformation::updateFromMouseEvent(QMouseEvent *event)
{
	m_mouseInfo.button = event->button();
	m_mouseInfo.buttons = event->buttons();
	
	m_position = event->pos();
	
	m_keyModifiers = event->modifiers();
	
	m_tabletInfo.pressure = -1;
	m_tabletInfo.rotation = 0;
	m_tabletInfo.tangentialPressure = -1;
}

void KTInputDeviceInformation::updateFromTabletEvent(QTabletEvent *event)
{
	m_tabletInfo.pressure = event->pressure();
	m_tabletInfo.rotation = event->rotation();
	m_tabletInfo.tangentialPressure = event->tangentialPressure();
	
	m_position = event->pos();
	
	m_keyModifiers = event->modifiers();
}


double KTInputDeviceInformation::pressure() const
{
	return m_tabletInfo.pressure;
}

double KTInputDeviceInformation::rotation() const
{
	return m_tabletInfo.rotation;
}

double KTInputDeviceInformation::tangentialPressure() const
{
	return m_tabletInfo.tangentialPressure;
}

Qt::MouseButton KTInputDeviceInformation::button() const
{
	return m_mouseInfo.button;
}

Qt::MouseButtons KTInputDeviceInformation::buttons() const
{
	return m_mouseInfo.buttons;
}


QPoint KTInputDeviceInformation::pos() const
{
	return m_position;
}

Qt::KeyboardModifiers KTInputDeviceInformation::keyModifiers() const
{
	return m_keyModifiers;
}




