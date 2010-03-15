/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#ifndef KTBUTTONITEM_H
#define KTBUTTONITEM_H

#include <QGraphicsItem>
#include <QIcon>
#include <QFont>

#include <ktabstractserializable.h>
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTButtonItem : public QObject, public QGraphicsItem, public KTAbstractSerializable
{
	Q_OBJECT;
	public:
		KTButtonItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		~KTButtonItem();
		
		virtual void fromXml(const QString &xml);
		virtual QDomElement toXml(QDomDocument &doc) const;
		
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w);
		QRectF boundingRect() const;
		
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
		
	public:
		void setIconSize(const QSize &size);
		void setIcon(const QIcon &icon);
		void setText(const QString &text);
		void setFont(const QFont &font);
		
	signals:
		void clicked();
		
	private:
		QSize m_iconSize;
		QIcon m_icon;
		QString m_text;
		QFont m_font;
};

#endif
