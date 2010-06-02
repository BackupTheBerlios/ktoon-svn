/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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
 *                                                                         *
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

#include "kcontrolnode.h"
#include "knodegroup.h"
#include <kcore/kdebug.h>

#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>
#include <QCursor>
#include <QGraphicsPathItem>

struct KControlNode::Private
{
    int index;
    QGraphicsItem * parent;
    KControlNode *nodeParent;
    KControlNode *left;
    KControlNode *right;
    bool notChange;
    KNodeGroup *nodeGroup;
    QGraphicsScene *scene;
};

KControlNode::KControlNode(int index, KNodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * parent,  
                           QGraphicsScene * scene) : QGraphicsItem(0, scene), k(new Private)
{
    k->index  = index;
    k->parent = 0;
    k->left = 0;
    k->right = 0;
    k->nodeParent = 0;
    k->notChange = true;
    k->nodeGroup = nodeGroup;
    k->scene = scene;
    
    QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor));
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);
    
    setPos(pos);
    setZValue(1000);
    changeParent(parent);
}

KControlNode::~KControlNode()
{    
}

void KControlNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    Q_UNUSED(w);
    
    bool antialiasing =  painter->renderHints() & QPainter::Antialiasing;
    
    painter->setRenderHint(QPainter::Antialiasing, false);
    
    QColor c;
    
    if (option->state & QStyle::State_Sunken || option->state & QStyle::State_Selected) {
        painter->save();
        painter->setPen(QPen(Qt::gray));
        painter->restore();
        
        if (QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(k->parent)) {
            QColor nc = it->pen().brush().color();
            c = QColor(nc.red()+180 % 255, nc.green()+180 % 255, nc.blue()+180 % 255);
        } else {
            c = QColor("green");
        }
        
        c.setAlpha(150);
    } else {
        if (QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(k->parent)) {
           QColor nc = it->pen().brush().color();
           c = QColor(nc.red()+254 % 255, nc.green()+254 % 255, nc.blue()+254 % 255);
        } else {
           c = QColor("navy");
        }
        
        c.setAlpha(150);
    }
    
    
    if (k->nodeParent)
        c.setRed(100);

    painter->setBrush(c);
    paintLinesToChilds(painter);
    
    painter->drawRoundRect(boundingRect());
    painter->setRenderHint(QPainter::Antialiasing, antialiasing);
}

void KControlNode::paintLinesToChilds(QPainter * painter)
{
    QMatrix inverted = sceneMatrix().inverted();
    painter->save();
    
    painter->setPen(QPen(QColor(0x8080FF)));
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (k->right) {
        if (k->right->isVisible())
            painter->drawLine(inverted.map(pos()), inverted.map(k->right->pos()));
    }

    if (k->left) {
        if (k->left->isVisible())
            painter->drawLine(inverted.map(pos()), inverted.map(k->left->pos()));
    }
    
    painter->restore();
}

QRectF KControlNode::boundingRect() const
{
    QSizeF size(8 , 8);
    QRectF r(QPointF(-size.width()/2, -size.height()/2), size);

    if (k->right) {
        if (k->right->isVisible())
            r.unite(k->right->boundingRect());
    }

    if (k->left) {
        if (k->left->isVisible())
           r.unite(k->left->boundingRect());
    }

    return r;
}

QVariant KControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    kFatal() << "1 KControlNode::itemChange -> Just starting!";

    if (change == QGraphicsItem::ItemPositionChange) {
        if (!k->notChange) {
            if (qgraphicsitem_cast<QGraphicsPathItem*>(k->parent)) {

                kFatal() << "2 KControlNode::itemChange -> ItemPositionChange";

                QPointF diff = value.toPointF() - pos();
                if (k->left)
                    k->left->moveBy(diff.x(), diff.y());

                if (k->right)
                    k->right->moveBy(diff.x(), diff.y());

                QPointF scenePos = k->parent->mapFromScene(value.toPointF());

                if (k->nodeGroup)
                    k->nodeGroup->moveElementTo(k->index, scenePos);
                else
                    kFatal() << "3 KControlNode::itemChange -> No k->nodeGroup";
           }
        } else {
           k->notChange = false;
           kFatal() << "4 KControlNode::itemChange -> notChange = false";
        }
    } else if (change == QGraphicsItem::ItemSelectedChange) {

               kFatal() << "5 KControlNode::itemChange -> ItemSelectedChange";

               if (value.toBool()) {
                   kFatal() << "6 KControlNode::itemChange -> value = true";
                   k->parent->setSelected(true);
                   setVisibleChilds(true);
               } else {

                   kFatal() << "7 KControlNode::itemChange -> value = false";
                   if (k->left) {
                       if (k->left->isSelected())
                           k->left->setVisible(true);
                       else
                           k->left->setVisible(false);
                   }

                   if (k->right) {
                       if (k->right->isSelected())
                           k->right->setVisible(true);
                       else
                           k->right->setVisible(false);
                   }
                   update();
               }
    }

    kFatal() << "8 KControlNode::itemChange -> Done";

    return QGraphicsItem::itemChange(change, value);
}

void KControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (k->nodeParent) {

        setSelected(true);
        k->nodeParent->setSelected(true);

        if (k->nodeParent->left()) {
            if (k->nodeParent->left() != this)
                k->nodeParent->left()->setSelected(false);
        }

        if (k->nodeParent->right()) {
            if (k->nodeParent->right() != this)
                k->nodeParent->right()->setSelected(false);
        }

    } else {
        setSeletedChilds(false);
    }

    QGraphicsItem::mousePressEvent(event);
    
    k->parent->setSelected(true);
    setVisibleChilds(true);
    
    event->accept();
}

void KControlNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    kFatal() << "KControlNode::mouseReleaseEvent -> CALLING INFANTRY!";

    k->nodeGroup->emitNodeClicked();

    // TODO: Why this instruction makes the system crash in Qt 4.5? 
    QGraphicsItem::mouseReleaseEvent(event);
}

void KControlNode::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    kFatal() << "KControlNode::mouseMoveEvent -> Moving node!";

    foreach (QGraphicsItem *item, scene()->selectedItems()) {
             if (qgraphicsitem_cast<KControlNode*>(item)) {
                 // TODO: Change this ugly if
                 if (k->nodeParent) { 
                     kFatal() << "KControlNode::mouseMoveEvent -> there's node parent";
                 } else {
                     if (item != this)
                         item->moveBy(event->pos().x(), event->pos().y());
                     else
                         kFatal() << "KControlNode::mouseMoveEvent -> Item IS this!";
                 }
             } else {
                 kFatal() << "KControlNode::mouseMoveEvent -> no control node ";
             }
    }

    setPos(event->scenePos());
    event->accept();
}

void KControlNode::setLeft(KControlNode *left)
{
    if (k->left)
        delete k->left;

    k->left = left;
    k->left->setVisible(false);
    k->left->setNodeParent(this);
    k->left->setZValue(zValue()+1);
}

void KControlNode::setRight(KControlNode *right)
{
    if (right)
        delete k->right;

    k->right = right;
    k->right->setVisible(false);
    k->right->setNodeParent(this);
    k->right->setZValue(zValue()+2);
}

void KControlNode::setNodeParent(KControlNode *nodeParent)
{
    k->nodeParent = nodeParent;
}

void KControlNode::setVisibleChilds(bool visible)
{
    if (k->left)
        k->left->setVisible(visible);

    if (k->right)
        k->right->setVisible(visible);
}

void KControlNode::setSeletedChilds(bool select)
{
    if (k->left)
        k->left->setSelected(select);

    if (k->right)
        k->right->setSelected(select);
}

KControlNode *KControlNode::left()
{
    return k->left;
}

KControlNode *KControlNode::right()
{
    return k->right;
}

KControlNode *KControlNode::nodeParent()
{
    return k->nodeParent;
}

int KControlNode::index() const
{
    return k->index;
}

void KControlNode::changeParent(QGraphicsItem *newParent)
{
    k->parent = newParent;
}

QGraphicsItem * KControlNode::parentI()
{
    return k->parent;
}

void KControlNode::setNotChange(bool notChange)
{
    k->notChange = notChange;
}
