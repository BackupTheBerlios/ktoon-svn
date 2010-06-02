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
    QGraphicsItem *graphicParent;
    KControlNode *centralNode;
    KControlNode *leftNode;
    KControlNode *rightNode;
    bool unchanged;
    KNodeGroup *nodeGroup;
    QGraphicsScene *scene;
};

KControlNode::KControlNode(int index, KNodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * graphicParent,  
                           QGraphicsScene * scene) : QGraphicsItem(0, scene), k(new Private)
{
    k->index  = index;
    k->graphicParent = 0;
    k->leftNode = 0;
    k->rightNode = 0;
    k->centralNode = 0;
    k->unchanged = true;
    k->nodeGroup = nodeGroup;
    k->scene = scene;
    
    QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor));
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);
    
    setPos(pos);
    // FIXME: The Zvalue for nodes must be relative to the QGraphicsItem variable
    setZValue(1000);
    setGraphicParent(graphicParent);
}

KControlNode::~KControlNode()
{    
}

void KControlNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    Q_UNUSED(w);
    
    // FIXME: Check Antialiasing management for this painter
    // bool antialiasing = painter->renderHints() & QPainter::Antialiasing;
    // painter->setRenderHint(QPainter::Antialiasing, antialiasing);
    
    QColor c;
    
    if (option->state & QStyle::State_Sunken || option->state & QStyle::State_Selected) {

        painter->save();
        painter->setPen(QPen(Qt::gray));
        painter->restore();
        
        if (QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(k->graphicParent)) {
            QColor nc = it->pen().brush().color();
            c = QColor(nc.red()+180 % 255, nc.green()+180 % 255, nc.blue()+180 % 255);
        } else {
            c = QColor("green");
        }
        c.setAlpha(150);

    } else {

        if (QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(k->graphicParent)) {
           QColor nc = it->pen().brush().color();
           c = QColor(nc.red()+254 % 255, nc.green()+254 % 255, nc.blue()+254 % 255);
        } else {
           c = QColor("navy");
        }
        
        c.setAlpha(150);
    }
    
    if (k->centralNode)
        c.setRed(100);

    painter->setBrush(c);
    paintLinesToChilds(painter);
    
    painter->drawRoundRect(boundingRect());
    // painter->setRenderHint(QPainter::Antialiasing, antialiasing);
}

void KControlNode::paintLinesToChilds(QPainter * painter)
{
    QMatrix inverted = sceneMatrix().inverted();
    painter->save();
    
    painter->setPen(QPen(QColor(0x8080FF)));
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (k->rightNode) {
        if (k->rightNode->isVisible())
            painter->drawLine(inverted.map(pos()), inverted.map(k->rightNode->pos()));
    }

    if (k->leftNode) {
        if (k->leftNode->isVisible())
            painter->drawLine(inverted.map(pos()), inverted.map(k->leftNode->pos()));
    }
    
    painter->restore();
}

QRectF KControlNode::boundingRect() const
{
    QSizeF size(8 , 8);
    QRectF r(QPointF(-size.width()/2, -size.height()/2), size);

    if (k->rightNode) {
        if (k->rightNode->isVisible())
            r.unite(k->rightNode->boundingRect());
    }

    if (k->leftNode) {
        if (k->leftNode->isVisible())
           r.unite(k->leftNode->boundingRect());
    }

    return r;
}

QVariant KControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    kFatal() << "1 KControlNode::itemChange -> Just starting!";

    if (change == QGraphicsItem::ItemPositionChange) {
        if (!k->unchanged) {
            if (qgraphicsitem_cast<QGraphicsPathItem*>(k->graphicParent)) {

                kFatal() << "2 KControlNode::itemChange -> ItemPositionChange";

                QPointF diff = value.toPointF() - pos();
                if (k->leftNode)
                    k->leftNode->moveBy(diff.x(), diff.y());

                if (k->rightNode)
                    k->rightNode->moveBy(diff.x(), diff.y());

                QPointF scenePos = k->graphicParent->mapFromScene(value.toPointF());

                if (k->nodeGroup) {
                    k->nodeGroup->moveElementTo(k->index, scenePos);
                    kFatal() << "2A KControlNode::itemChange -> Moving point!!!";
                } else {
                    kFatal() << "3 KControlNode::itemChange -> No k->nodeGroup";
                }
           } else {
                kFatal() << "3A KControlNode::itemChange -> cast for QGraphicsPathItem has failed!";
           }
        } else {
           k->unchanged = false;
           kFatal() << "4 KControlNode::itemChange -> no Change";
        }
    } else if (change == QGraphicsItem::ItemSelectedChange) {

               kFatal() << "5 KControlNode::itemChange -> ItemSelectedChange";

               if (value.toBool()) {
                   kFatal() << "6 KControlNode::itemChange -> value = true";
                   k->graphicParent->setSelected(true);
                   setVisibleChilds(true);
               } else {

                   kFatal() << "7 KControlNode::itemChange -> value = false";
                   if (k->leftNode) {
                       if (k->leftNode->isSelected())
                           k->leftNode->setVisible(true);
                       else
                           k->leftNode->setVisible(false);
                   }

                   if (k->rightNode) {
                       if (k->rightNode->isSelected())
                           k->rightNode->setVisible(true);
                       else
                           k->rightNode->setVisible(false);
                   }
                   update();
               }
    }

    kFatal() << "8 KControlNode::itemChange -> Done";

    return QGraphicsItem::itemChange(change, value);
}

void KControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (k->centralNode) {

        setSelected(true);
        k->centralNode->setSelected(true);

        if (k->centralNode->left()) {
            if (k->centralNode->left() != this)
                k->centralNode->left()->setSelected(false);
        }

        if (k->centralNode->right()) {
            if (k->centralNode->right() != this)
                k->centralNode->right()->setSelected(false);
        }

    } else {
        setSeletedChilds(false);
    }

    QGraphicsItem::mousePressEvent(event);
    
    k->graphicParent->setSelected(true);
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
                 if (k->centralNode) { 
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
    if (k->leftNode)
        delete k->leftNode;

    k->leftNode = left;
    k->leftNode->setVisible(false);
    k->leftNode->setCentralNode(this);
    k->leftNode->setZValue(zValue()+1);
}

void KControlNode::setRight(KControlNode *right)
{
    if (right)
        delete k->rightNode;

    k->rightNode = right;
    k->rightNode->setVisible(false);
    k->rightNode->setCentralNode(this);
    k->rightNode->setZValue(zValue()+2);
}

void KControlNode::setCentralNode(KControlNode *centralNode)
{
    k->centralNode = centralNode;
}

void KControlNode::setVisibleChilds(bool visible)
{
    if (k->leftNode)
        k->leftNode->setVisible(visible);

    if (k->rightNode)
        k->rightNode->setVisible(visible);
}

void KControlNode::setSeletedChilds(bool select)
{
    if (k->leftNode)
        k->leftNode->setSelected(select);

    if (k->rightNode)
        k->rightNode->setSelected(select);
}

KControlNode *KControlNode::left()
{
    return k->leftNode;
}

KControlNode *KControlNode::right()
{
    return k->rightNode;
}

KControlNode *KControlNode::centralNode()
{
    return k->centralNode;
}

int KControlNode::index() const
{
    return k->index;
}

void KControlNode::setGraphicParent(QGraphicsItem *newParent)
{
    k->graphicParent = newParent;
}

QGraphicsItem * KControlNode::parentI()
{
    return k->graphicParent;
}

void KControlNode::setNotChange(bool unchanged)
{
    k->unchanged = unchanged;
}
