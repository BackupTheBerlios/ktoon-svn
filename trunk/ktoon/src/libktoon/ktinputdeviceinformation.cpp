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
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QTabletEvent>


struct KTInputDeviceInformation::Private
{
	struct TabletInfo
	{
		double pressure;
		double rotation;
		double tangentialPressure;
	} tabletInfo;
	
	struct MouseInfo
	{
		Qt::MouseButton button;
		Qt::MouseButtons buttons;
	} mouseInfo;
	
	QPointF position;
	Qt::KeyboardModifiers keyModifiers;
};

KTInputDeviceInformation::KTInputDeviceInformation(QObject *parent) : QObject(parent), d(new Private)
{
	d->mouseInfo.button = Qt::NoButton;
	d->mouseInfo.buttons = Qt::NoButton;
	
	d->tabletInfo.pressure = -1;
	d->tabletInfo.rotation = 0;
	d->tabletInfo.tangentialPressure = -1;
	
	d->keyModifiers = Qt::NoModifier;
}


KTInputDeviceInformation::~KTInputDeviceInformation()
{
	delete d;
}

void KTInputDeviceInformation::updateFromMouseEvent(QGraphicsSceneMouseEvent *event)
{
	d->mouseInfo.button = event->button();
	d->mouseInfo.buttons = event->buttons();
	
	d->position = event->scenePos();
	
	d->keyModifiers = event->modifiers();
	
	d->tabletInfo.pressure = -1;
	d->tabletInfo.rotation = 0;
	d->tabletInfo.tangentialPressure = -1;
}

void KTInputDeviceInformation::updateFromMouseEvent(QMouseEvent *event)
{
	d->mouseInfo.button = event->button();
	d->mouseInfo.buttons = event->buttons();
	
	d->position = event->pos();
	
	d->keyModifiers = event->modifiers();
	
	d->tabletInfo.pressure = -1;
	d->tabletInfo.rotation = 0;
	d->tabletInfo.tangentialPressure = -1;
}

void KTInputDeviceInformation::updateFromTabletEvent(QTabletEvent *event)
{
	d->tabletInfo.pressure = event->pressure();
	d->tabletInfo.rotation = event->rotation();
	d->tabletInfo.tangentialPressure = event->tangentialPressure();
	
	d->position = event->pos();
	
	d->keyModifiers = event->modifiers();
}


double KTInputDeviceInformation::pressure() const
{
	return d->tabletInfo.pressure;
}

double KTInputDeviceInformation::rotation() const
{
	return d->tabletInfo.rotation;
}

double KTInputDeviceInformation::tangentialPressure() const
{
	return d->tabletInfo.tangentialPressure;
}

Qt::MouseButton KTInputDeviceInformation::button() const
{
	return d->mouseInfo.button;
}

Qt::MouseButtons KTInputDeviceInformation::buttons() const
{
	return d->mouseInfo.buttons;
}


QPointF KTInputDeviceInformation::pos() const
{
	return d->position;
}

Qt::KeyboardModifiers KTInputDeviceInformation::keyModifiers() const
{
	return d->keyModifiers;
}




