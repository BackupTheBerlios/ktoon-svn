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

#ifndef KTPATHITEM_H
#define KTPATHITEM_H

#include "ktabstractserializable.h"
#include <QGraphicsPathItem>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTPathItem : public KTAbstractSerializable, public QGraphicsPathItem
{
	public:
		KTPathItem( QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		~KTPathItem();
		
		void fromXml(const QString &xml);
		QDomElement toXml(QDomDocument &doc);
		
	protected:
		virtual void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
		virtual void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
		virtual void dropEvent ( QGraphicsSceneDragDropEvent *event );
		
	private:
		bool m_dragOver;

};

#endif

