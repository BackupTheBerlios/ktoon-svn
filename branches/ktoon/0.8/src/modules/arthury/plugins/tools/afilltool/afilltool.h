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

#ifndef AFILLTOOL_H
#define AFILLTOOL_H

#include <kttoolpluginobject.h>
#include <atoolinterface.h>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class AFillTool : public KTToolPluginObject, public AToolInterface
{
	Q_OBJECT;
	Q_INTERFACES(AToolInterface);
	public:
		AFillTool();
		~AFillTool();

		virtual QHash< QString, DAction * > actions();
		virtual QPainterPath path() const;
		virtual QRect move(const QString& brush, QPainter& painter, const QPoint& oldPos, const QPoint& newPos);
		virtual QRect press(const QString& brush, QPainter& painter, const QPoint& pos, KTKeyFrame* currentFrame);
		virtual QRect release(const QString &brush, QPainter &painter,const QPoint &pos);
		virtual QStringList keys() const;
		virtual QWidget *configurator();
		virtual int type() const;
		virtual bool isComplete() const;
		virtual void aboutToChangeTool();
		
	private:
		void buildPath(AGraphic *current,  QPainterPath &fillPath, QList<AGraphic *> &graphics);
		void joinPath(QPainterPath &orig, const QPainterPath &toJoin);
		
		bool isIntersected(const QPointF &point, const QPolygonF &polygon);
		bool isContained(const QPointF &point, const QPainterPath &path);
		bool isClosed(const QPainterPath &path);
};

#endif
