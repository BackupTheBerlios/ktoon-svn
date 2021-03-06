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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "node.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>

#include <QCursor>
#include <kcore/kdebug.h>
#include "nodemanager.h"

#include <cmath> //atan
#include "ktgraphicalgorithm.h"

#include "ktgraphicobject.h"

#define DEBUG 0

/**
 * This class defines the data structure for a node, and all the methods required to manipulate it.
 * 
 * @author David Cuadrado <krawek@toonka.com>
*/

struct Node::Private
{
    Private(TypeNode node, ActionNode action, NodeManager *manager, QGraphicsItem * parent) : typeNode(node),
                        action(action), notChange(false), parent(parent), manager(manager)
    {
    }
    
    TypeNode typeNode;
    ActionNode action;
    bool notChange;
    QGraphicsItem * parent;
    NodeManager *manager;
};

Node::Node(TypeNode node, ActionNode action, const QPointF & pos, NodeManager *manager, QGraphicsItem * parent,
        QGraphicsScene * scene) : QGraphicsItem(0, scene), k(new Private(node, action, manager, parent))
{
    QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor));
    // setParentItem(k->parent);
    setFlag(ItemIsSelectable, false);
    setFlag(ItemIsMovable, true);
    
    setPos(pos);
    setZValue(1000);
}

Node::~Node()
{
    delete k;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    Q_UNUSED(w);
    
    bool antialiasing =  painter->renderHints() & QPainter::Antialiasing;
    painter->setRenderHint(QPainter::Antialiasing, false);
    
    QColor c;
    
    if (option->state & QStyle::State_Sunken) {
        c = QColor("green");
        c.setAlpha(150);
    } else {
        c = QColor("navy");
        c.setAlpha(150);
    }
    
    if (k->action == Rotate)
        c.setGreen(200);

    QRectF br = boundingRect();

    painter->setBrush(c);
    painter->drawRoundRect(br);

    //DEBUG
    #if DEBUG
        painter->setFont(QFont( painter->font().family(), 5));
        painter->drawText(br, QString::number(k->typeNode));
    #endif

    if (k->typeNode == Center) {
        painter->save();
        painter->setPen(Qt::gray);
        painter->drawLine(br.topLeft(), br.bottomRight());
        painter->drawLine(br.bottomLeft(), br.topRight());
        painter->restore();
    }
    
    painter->setRenderHint(QPainter::Antialiasing, antialiasing);
}

QRectF Node::boundingRect() const
{
    QSizeF size(8, 8);
    QRectF r(QPointF(-size.width()/2, -size.height()/2), size);

    return r;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change ==  ItemSelectedChange) {
        K_FUNCINFO;
        setVisible(true);
        
        if (value.toBool())
            k->parent->setSelected(true);

        k->manager->show();
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    K_FUNCINFO;
    // update();
    k->manager->setPress(true);
    
    QGraphicsItem::mousePressEvent(event);
    
    #if DEBUG
        QRectF r = k->parent->sceneMatrix().inverted().mapRect( k->parent->sceneBoundingRect());
        scene()->addRect(r, QPen(Qt::magenta), QBrush(QColor(100,100,200,50)));
        scene()->update(r);
    #endif
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    K_FUNCINFO;
    // update();
    QGraphicsItem::mouseReleaseEvent(event);
    k->parent->setSelected(true);
    k->manager->setPress(false);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF newPos(event->scenePos());

    if( k->notChange) {
        k->notChange = false;
    } else {
        if (k->action == Scale) {
            QRectF rect = k->parent->sceneBoundingRect();
            QRectF br   = k->parent->sceneBoundingRect();
            QRectF br1  = k->parent->boundingRect();
            
            //Debug
            /*
            scene()->addRect(rect, QPen(Qt::red));
            scene()->addRect(br, QPen(Qt::green));
            */
            
            switch (k->typeNode) {
                    case TopLeft:
                    {
                         k->manager->setAnchor(br1.bottomRight());
                         rect.setTopLeft(newPos);
                         break;
                    }
                    case TopRight:
                    {
                         k->manager->setAnchor(br1.bottomLeft());
                         rect.setTopRight(newPos);
                         break;
                    }
                    case BottomRight:
                    {
                         k->manager->setAnchor(br1.topLeft());
                         rect.setBottomRight(newPos);
                         break;
                    }
                    case BottomLeft:
                    {
                         k->manager->setAnchor(br1.topRight());
                         rect.setBottomLeft(newPos);
                         break;
                    }
                    case Center:
                    {
                         break;
                    }
            };
            
            float sx = 1, sy = 1;
            sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
            sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
            
            if (sx > 0 && sy > 0) {
                k->manager->scale( sx,sy);
            } else {
                if (sx > 0)
                    k->manager->scale(sx, 1);

                if (sy > 0)
                    k->manager->scale(1, sy);
            }
        } else {
            
            if (k->typeNode != Center) {
                // k->manager->setVisible(false);
                QPointF p1 = newPos;
                QPointF p2 = k->parent->sceneBoundingRect().center();
                k->manager->setAnchor(k->parent->boundingRect().center());
                
                double a = (180 * KTGraphicalAlgorithm::angleForPos(p1, p2)) / M_PI;
                k->manager->rotate(a-45);
            }
        }
    }

    if (k->typeNode == Center) {
        k->parent->moveBy(event->scenePos().x() - scenePos().x() , event->scenePos().y() - scenePos().y());
        event->accept();
    }
}


void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * e)
{
    K_FUNCINFO;
    // update();
    k->manager->toggleAction();
    e->setAccepted(false);
    QGraphicsItem::mouseDoubleClickEvent(e);
}

int Node::typeNode() const
{
    return k->typeNode;
}

void Node::setAction(ActionNode action)
{
    k->action = action;
    update();
}

int Node::actionNode()
{
    return k->action;
}
