/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktproxyitem.h"
#include <QPainter>
#include <kcore/kdebug.h>

/**
 * This class defines a copy of a graphic object.
 * Here is where methods to handle a graphic object copy are defined.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTProxyItem::Private 
{
    Private() : realItem(0) {}
    QGraphicsItem *realItem;
};

KTProxyItem::KTProxyItem(QGraphicsItem *item) : QGraphicsItem(), k(new Private)
{
    setItem(item);
    setPos(0,0);
}

KTProxyItem::~KTProxyItem()
{
}

void KTProxyItem::setItem(QGraphicsItem *item)
{
    if (k->realItem)
        this->removeSceneEventFilter(k->realItem);
    
    k->realItem = item;
    
    if (k->realItem) {
        k->realItem->installSceneEventFilter(this);
        this->setFlags(k->realItem->flags());
    }
}

QGraphicsItem *KTProxyItem::item() const
{
    return k->realItem;
}

QRectF KTProxyItem::boundingRect() const
{
    if (k->realItem) {
        QRectF tmp = k->realItem->boundingRect();
        kFatal() << "Width: " << tmp.width();
        kFatal() << "Height: " << tmp.height();
        return k->realItem->boundingRect();
    }
    
    return QRectF(0, 0, 0, 0);
}

void KTProxyItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (k->realItem)
        k->realItem->paint(painter, option, widget);
}

QPainterPath KTProxyItem::shape() const
{
    if (k->realItem)
        return k->realItem->shape();
    
    return QGraphicsItem::shape();
}

bool KTProxyItem::collidesWithItem(const QGraphicsItem * other, Qt::ItemSelectionMode mode) const
{
    if (k->realItem)
        return k->realItem->collidesWithItem(other, mode);
    
    return QGraphicsItem::collidesWithItem(other, mode);
}

bool KTProxyItem::collidesWithPath(const QPainterPath & path, Qt::ItemSelectionMode mode) const
{
    if (k->realItem)
        return k->realItem->collidesWithPath(path, mode);
    
    return QGraphicsItem::collidesWithPath(path, mode);
}

bool KTProxyItem::contains(const QPointF & point) const
{
    if (k->realItem)
        return k->realItem->contains(point);
    
    return QGraphicsItem::contains(point);
}

bool KTProxyItem::isObscuredBy(const QGraphicsItem * item) const
{
    if (k->realItem)
        return k->realItem->isObscuredBy(item);
    
    return QGraphicsItem::isObscuredBy(item);
}

QPainterPath KTProxyItem::opaqueArea() const
{
    if (k->realItem)
        return k->realItem->opaqueArea();
    
    return QGraphicsItem::opaqueArea();
}
