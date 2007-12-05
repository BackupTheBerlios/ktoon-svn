/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrox@toonka.com>             *
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

#ifndef AFILTERINTERFACE_H
#define AFILTERINTERFACE_H

#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QHash>

#include <cmath> // sin,cos

#include "ktframe.h"
#include <kgui/kaction.h>

#include "qplugin.h" // Q_EXPORT_PLUGIN

class QKeySequence;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT AFilterInterface
{
	public:
		virtual ~AFilterInterface() {}
		virtual QStringList keys() const = 0;
// 		virtual void filter(const QString &filter, const QList<KTGraphicComponent *> &frame) = 0;
		
		virtual QHash<QString, KAction *>actions() = 0;
};

Q_DECLARE_INTERFACE(AFilterInterface, "com.toonka.ktoon.AFilterInterface/0.1");

#endif
