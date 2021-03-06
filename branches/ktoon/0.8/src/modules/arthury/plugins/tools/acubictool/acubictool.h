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
 

#ifndef ACUBICTOOL_H
#define ACUBICTOOL_H

#include <kttoolpluginobject.h>
#include <atoolinterface.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */

class ACubicTool : public KTToolPluginObject, public AToolInterface
{
	Q_OBJECT
	Q_INTERFACES(AToolInterface);
	public:
		ACubicTool();
		~ACubicTool();
		
		virtual QHash< QString, DAction * > actions();
		virtual QPainterPath path() const;
		virtual QRect move(const QString& brush, QPainter& painter, const QPoint& oldPos, const QPoint& newPos);
		virtual QRect press(const QString& brush, QPainter& painter, const QPoint& pos, KTKeyFrame* currentFrame);
		virtual QRect release(const QString& brush, QPainter& painter, const QPoint& pos);
		virtual QStringList keys() const;
		virtual QWidget * configurator();
		virtual int type() const;
		virtual bool isComplete() const;
		virtual void aboutToChangeTool();
		
	private:
		QPainterPath m_path;
		
		struct Node;
		struct Node
		{
			QPoint right, center, left;
		};
		int m_count;
		Node *m_root;
		QList< Node > m_nodes;
		void createFinishPath();
		
		bool m_isComplete;
};

#endif
