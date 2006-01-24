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

#ifndef AGEOMETRICTOOLPLUGIN_H
#define AGEOMETRICTOOLPLUGIN_H

#include <atoolinterface.h>
#include <QObject>
#include <QLabel>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class AGeometricToolPlugin : public KTToolPluginObject, public AToolInterface
{
	Q_OBJECT;
	Q_INTERFACES(AToolInterface);
	
	public:
		virtual QStringList keys() const;
		virtual QRect press(const QString &brush, QPainter &painter, const QPainterPath &form, const QPoint &pos, KTKeyFrame *currentFrame = 0);
		virtual QRect move(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos);
		virtual QRect release(const QString &brush, QPainter &painter, const QPainterPath &form, const QPoint &pos);
		virtual QPainterPath path() const;

		virtual QHash<QString, QAction *>actions();
		
		int type() const
		{
			return Brush;
		}
		
		virtual QWidget *configurator()
		{
			return  0;
		}
		
	private:
		QPainterPath m_path;
		QRect m_rect;
};

#endif
