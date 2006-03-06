/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#ifndef APOLYGONTOOL_H
#define APOLYGONTOOL_H

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
#include <kttoolpluginobject.h>
#include <atoolinterface.h>
#include <QPainterPath>

class APolygonTool: public KTToolPluginObject, public AToolInterface
{
	Q_OBJECT;
	Q_INTERFACES(AToolInterface);
	public:
		APolygonTool();
		~APolygonTool();
		
		QHash< QString, KTAction * >  actions();

		QPainterPath  path() const;

		QRect  move(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& oldPos, const QPoint& newPos);

		QRect  press(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos, KTKeyFrame* currentFrame);

		QRect  release(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos);
		
		QStringList  keys() const;

		QWidget*  configurator();

		bool  isComplete() const;

		int type() const;

		void aboutToChangeTool();
	private:
		QPainterPath m_shape;
		QPoint m_initialPoint;
		
	private:
		QPainterPath createShape(const QRect & rect);

};

#endif
