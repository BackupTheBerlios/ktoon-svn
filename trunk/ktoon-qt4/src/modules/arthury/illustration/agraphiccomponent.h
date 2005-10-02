/***************************************************************************
 *   Copyright (C) 2004 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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
 
#ifndef AGRAPHICCOMPONENT_H
#define AGRAPHICCOMPONENT_H

#include <QObject>
#include <QPainterPath>
#include <QBrush>
#include <QPen>

#include <QDomElement>
#include <QDomDocument>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class AGraphicComponent : public QObject
{
	Q_OBJECT
	public:
		AGraphicComponent(QObject *parent = 0);
		virtual ~AGraphicComponent();
		
		virtual QDomElement createXML( QDomDocument &doc ) = 0;
		virtual QString key() const = 0;
		
		QRectF boundingRect() const;
		QColor color() const;
		QPainterPath path() const;
		QBrush brush() const;
		QPen pen() const;
		
		virtual void setPath(const QPainterPath &path );
		virtual void setColor(const QColor &color);
		virtual void setBrush(const QBrush &brush);
		virtual void setPen(const QPen &pen);
		virtual void setPen(const QColor &color);
		
	protected:
		QPainterPath m_pPath;
		QColor m_pColor;
		QBrush m_pBrush;
		QPen m_pPen;
};

#endif
