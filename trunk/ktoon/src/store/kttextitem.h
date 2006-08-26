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

#ifndef KTTEXTITEM_H
#define KTTEXTITEM_H

#include <QGraphicsTextItem>
#include <ktabstractserializable.h>
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class STORE_EXPORT KTTextItem : public QGraphicsTextItem, public KTAbstractSerializable
{
	Q_OBJECT;
	public:
		KTTextItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		~KTTextItem();
		
		virtual void fromXml(const QString &xml);
		virtual QDomElement toXml(QDomDocument &doc);
		
		void setEditable(bool editable);
		
	public slots:
		void toggleEditable();
		
	signals:
		void edited();
		
	protected:
		virtual void focusOutEvent(QFocusEvent * event );
		virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
		
	private:
		GraphicsItemFlags m_flags;
		bool m_isEditable;
};

#endif
