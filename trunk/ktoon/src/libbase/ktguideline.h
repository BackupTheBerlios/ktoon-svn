/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   kruadrosx@toonka.com                                                     *
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
 
#ifndef KTLINEGUIDE_H
#define KTLINEGUIDE_H

#include <QGraphicsItem>


/**
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
*/
class KTLineGuide : public QGraphicsItem
{
	public:
		KTLineGuide(Qt::Orientation o, QGraphicsScene *scene);
		~KTLineGuide();
		
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
		
	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant & value);
		
// 		void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		
		bool sceneEvent(QEvent *e);
		
	private:
		struct Private;
		Private * const d;
		
		void syncCursor();
		
};

#endif
