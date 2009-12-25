/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               * 
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
#include "ktpaintareabase.h"

#include <QTimer>

struct KTPaintAreaRotator::Private
{
    int rotationAngle;
    KTPaintAreaBase *view;
    
    QTimer timer;
};

KTPaintAreaRotator::KTPaintAreaRotator(QObject *parent, KTPaintAreaBase *view) : QObject(parent), k(new Private)
{
    k->view = view;
    connect(&k->timer, SIGNAL(timeout()), this, SLOT(applyRotation()));
}

KTPaintAreaRotator::~KTPaintAreaRotator()
{
    delete k;
}

void KTPaintAreaRotator::rotateTo(int angle)
{
    k->rotationAngle = angle;
    
    if (!k->timer.isActive())
        k->timer.start(50);
}

void KTPaintAreaRotator::applyRotation()
{
    k->view->setRotationAngle(k->rotationAngle);
    k->timer.stop();
}

