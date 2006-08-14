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

#ifndef ATOOLINTERFACE_H
#define ATOOLINTERFACE_H

#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QImage>
#include <QHash>
#include <QCursor>
#include <QMouseEvent>

#include "ktframe.h"

#include "daction.h"

#include "qplugin.h" // Q_EXPORT_PLUGIN
#include "ktglobal.h"

class KTScene;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTOON_EXPORT KTToolInterface
{
	public:
		enum ToolType
		{
			None = 0,
			Brush,
			Fill,
			Selection
		};
		
		virtual ~KTToolInterface() {};
		
		virtual QStringList keys() const = 0;
		virtual int toolType() const = 0;
};

Q_DECLARE_INTERFACE( KTToolInterface, "com.toonka.ktoon.KTToolInterface/0.2" );

#endif
