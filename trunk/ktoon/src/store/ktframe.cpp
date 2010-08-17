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

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QCursor>
#include <QSvgRenderer>
#include <kcore/kdebug.h>

#include "ktframe.h"
#include "ktlayer.h"

#include "ktitemfactory.h"
#include "ktserializer.h"
#include "ktgraphicobject.h"
#include "ktgraphiclibraryitem.h"
#include "ktlibrary.h"

#include "ktitemgroup.h"
#include "ktitemtweener.h"

#include "ktprojectloader.h"

#include "ktscene.h"
#include "ktlayer.h"

struct KTFrame::Private
{
    QString name;
    bool isLocked;
    bool isVisible;
    GraphicObjects graphics;
    SvgObjects svg;
    int repeat;
    int zLevelIndex;
};

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), k(new Private)
{
    k->name = "Frame";
    k->isLocked = false;
    k->isVisible = true;
    k->repeat = 1;
    k->zLevelIndex = -1;
}

KTFrame::~KTFrame()
{
    k->graphics.clear(true);
    k->svg.clear(true);
    delete k;
}

void KTFrame::clean()
{
    k->graphics.clear(true);
    k->svg.clear(true);
}

void KTFrame::setFrameName(const QString &name)
{
    k->name = name;
}

void KTFrame::setLocked(bool isLocked)
{
    k->isLocked = isLocked;
}

QString KTFrame::frameName() const
{
    return k->name;
}

bool KTFrame::isLocked() const
{
    return k->isLocked;
}

void KTFrame::setVisible(bool isVisible)
{
    k->isVisible = isVisible;
}

bool KTFrame::isVisible() const
{
    return k->isVisible;
}

void KTFrame::fromXml(const QString &xml)
{
    QDomDocument document;
	
    if (! document.setContent(xml))
        return;

    QDomElement root = document.documentElement();
    setFrameName(root.attribute("name", frameName()));

    QDomNode n = root.firstChild();

    while (!n.isNull()) {
           QDomElement e = n.toElement();

           if (!e.isNull()) {

               if (e.tagName() == "object") {
                   QDomNode n2 = e.firstChild();

                   KTGraphicObject *last = 0; // SQA: Verify this weird variable

                   while (!n2.isNull()) {
                          QDomElement e2 = n2.toElement();

                          if (e2.tagName() == "tweening" && last) {
                              kFatal() << "Reading tween from file!";
                              KTItemTweener *tweener = new KTItemTweener(0, last);
                              QString newDoc;

                              {
                                QTextStream ts(&newDoc);
                                ts << n2;
                              }

                              tweener->fromXml(newDoc);
                              last->setTweener(tweener);
                          } else {
                              QString newDoc;

                              {
                                QTextStream ts(&newDoc);
                                ts << n2;
                              }

                              createItem(k->graphics.count(), QPointF(), newDoc);
                              //last = k->graphics.value(k->graphics.count()-1);
                          }
                          n2 = n2.nextSibling();
                   }
               } else if (e.tagName() == "svg") {

                          QString path = e.attribute("itemPath");
                          QDomNode n2 = e.firstChild();

                          while (!n2.isNull()) {
                                 QDomElement e2 = n2.toElement();

                                 if (e2.tagName() == "properties") {
                              
                                     QString newDoc;
                                     {
                                       QTextStream ts(&newDoc);
                                       ts << n2;
                                     }

                                     KTSvgItem *svg = new KTSvgItem(path);
                                     KTSerializer::loadProperties(svg, e2);
                                     insertSvgItem(k->svg.count(), svg);
                                 }

                                 n2 = n2.nextSibling(); 
                          }
               }
           }
		
           n = n.nextSibling();
    }
}

QDomElement KTFrame::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("frame");
    root.setAttribute("name", k->name);
    doc.appendChild(root);

    foreach (KTGraphicObject *object, k->graphics.values())
             root.appendChild(object->toXml(doc));

    foreach (KTSvgItem *object, k->svg.values())
             root.appendChild(object->toXml(doc));

    return root;
}

void KTFrame::addItem(QGraphicsItem *item)
{
    insertItem(k->graphics.count(), item);
}

void KTFrame::addSvgItem(KTSvgItem *item)
{
    insertSvgItem(k->svg.count(), item);
}

void KTFrame::insertItem(int position, QGraphicsItem *item)
{
    if (k->graphics.contains(position-1)) {
        if (QGraphicsItem *lastItem = k->graphics.value(position-1)->item())
            k->zLevelIndex = lastItem->zValue()+1; 
            item->setZValue(k->zLevelIndex);
    } else {
        if (k->zLevelIndex == -1)
            k->zLevelIndex = item->zValue();
    }

    KTGraphicObject *object = new KTGraphicObject(item, this);
    //object->setObjectName(id);
    k->graphics.insert(position, object);
}

void KTFrame::insertSvgItem(int position, KTSvgItem *item)
{
    if (k->svg.contains(position-1)) {
        if (KTSvgItem *lastItem = k->svg.value(position-1))
            k->zLevelIndex = lastItem->zValue()+1;
            item->setZValue(k->zLevelIndex);
    } else {
        if (k->zLevelIndex == -1)
            k->zLevelIndex = item->zValue();
    }

    k->svg.insert(position, item);
}


QGraphicsItemGroup *KTFrame::createItemGroupAt(int position, QList<qreal> group)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    qSort(group.begin(), group.end());

    KTItemGroup *itemGroup = new KTItemGroup(0);
    int count = 0;

    foreach (qreal p, group) {
             int pos = (int)p - count;
             QGraphicsItem *item = this->item(pos);
             KTGraphicObject *object = this->graphic(pos);
             object->setItem(0);
             removeGraphicAt(pos);
             itemGroup->addToGroup(item);
             delete object;
             count++;
    }

    QGraphicsItem *block = qgraphicsitem_cast<QGraphicsItem *>(itemGroup);

    insertItem(position, block);

    return itemGroup;
}

QList<QGraphicsItem *> KTFrame::destroyItemGroup(int position)
{
    QList<QGraphicsItem *> items;

    if (KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item(position))) {
        removeGraphicAt(position);
        items = group->childs();
        foreach (QGraphicsItem *child, group->childs()) {
                 group->removeFromGroup(child);
                 addItem(child);
        }
    }

    return items;
}

void KTFrame::replaceItem(int position, QGraphicsItem *item)
{
    KTGraphicObject *toReplace = this->graphic(position);

    if (toReplace)
        toReplace->setItem(item);
}

bool KTFrame::moveItem(int currentPosition, int newPosition)
{
    #ifdef K_DEBUG
           K_FUNCINFO << "current "<< currentPosition << " new "  << newPosition;
    #endif

    if (currentPosition == newPosition || currentPosition < 0 
        || currentPosition >= k->graphics.count() || newPosition < 0 
        || newPosition >= k->graphics.count())
        return false;

    if (currentPosition < newPosition) {
        for (int i = currentPosition; i < newPosition; i++) {
             double tmp = k->graphics.value(i)->item()->zValue();
             k->graphics.value(i)->item()->setZValue(k->graphics.value(i+1)->item()->zValue());
             k->graphics.value(i+1)->item()->setZValue(tmp);
             k->graphics.copyObject(i, i+1);
        }
    } else {
             for (int i = currentPosition; i > newPosition; i--) {
                  double tmp = k->graphics.value(i)->item()->zValue();
                  k->graphics.value(i)->item()->setZValue(k->graphics.value(i-1)->item()->zValue());
                  k->graphics.value(i-1)->item()->setZValue(tmp);
                  k->graphics.copyObject(i, i-1);
             }
    }

    return true;
}

bool KTFrame::removeGraphicAt(int position)
{
    if (position < 0)
        return false;

    KTGraphicObject *object = k->graphics.takeObject(position);

    if (object) {
        this->scene()->removeTweeningObject(object);
        return true;
    }

    /*
    QGraphicsItem *item = object->item();

    if (item) {
        QGraphicsScene *scene = item->scene();

        if (scene)
            scene->removeItem(item);

        this->scene()->removeTweeningObject(object);

        return true;
    }
    */

    return false;
}

bool KTFrame::removeSvgAt(int position)
{
    kFatal() << "KTFrame::removeSvgAt - Index: " <<  position;

    if (position < 0)
        return false;

    KTSvgItem *item = k->svg.takeObject(position);

    if (item) {
        QGraphicsScene *scene = item->scene();

        if (scene)
            scene->removeItem(item);
 
        // SQA: Pending to check
        //this->scene()->removeTweeningObject(item);

        return true;
    } else {
        kFatal() << "KTFrame::removeSvgAt - object not found :S"; 
    }

    return false;
}

QGraphicsItem *KTFrame::createItem(int position, QPointF coords, const QString &xml, bool loaded)
{
    KTItemFactory itemFactory;
    itemFactory.setLibrary(project()->library());
    QGraphicsItem *graphicItem = itemFactory.create(xml);

    if (graphicItem)
        insertItem(position, graphicItem);

    if (loaded)
        KTProjectLoader::createItem(scene()->objectIndex(), layer()->objectIndex(), index(), position, coords, KTLibraryObject::Item, xml, project());

    return graphicItem;
}

KTSvgItem *KTFrame::createSvgItem(int position, QPointF coords, const QString &xml, bool loaded)
{
    QDomDocument document;
    if (!document.setContent(xml))
        return 0;

    QDomElement root = document.documentElement();
    QString path = root.attribute("itemPath");

    KTSvgItem *item = new KTSvgItem(path);
    item->moveBy(coords.x(), coords.y()); 

    insertSvgItem(position, item);

    if (loaded)
        KTProjectLoader::createItem(scene()->objectIndex(), layer()->objectIndex(), index(), 
                                    position, coords, KTLibraryObject::Svg, xml, project());

    return item;
}

void KTFrame::setGraphics(GraphicObjects objects)
{
    k->graphics = objects;
}

void KTFrame::setSvgObjects(SvgObjects items)
{
    k->svg = items;
}

GraphicObjects KTFrame::graphics() const
{
    return k->graphics;
}

SvgObjects KTFrame::svgItems() const
{
    return k->svg;
}

KTGraphicObject *KTFrame::graphic(int position) const
{
    if (position < 0) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound [KTFrame->graphic()] - index: " << position << " - total items: " << k->graphics.count();
        #endif

        return 0;
    } 

    return k->graphics.value(position);
}

KTSvgItem *KTFrame::svg(int position) const
{
    if (position < 0) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound [KTFrame->svg()] - index: " << position << " - total items: " << k->svg.count();
        #endif

        return 0;
    }

    return k->svg.value(position);
}


QGraphicsItem *KTFrame::item(int position) const
{
    KTGraphicObject *object = graphic(position);

    if (object)
        return object->item();

    return 0;
}

int KTFrame::indexOf(KTGraphicObject *object) const
{
    return k->graphics.objectIndex(object);
}

int KTFrame::indexOf(KTSvgItem *object) const
{
    return k->svg.objectIndex(object);
}

int KTFrame::indexOf(QGraphicsItem *item) const
{
    foreach (KTGraphicObject *object, k->graphics.values()) {
             if (object->item() == item)
                 return k->graphics.objectIndex(object);
    }

    return -1;
}

void KTFrame::setRepeat(int repeat)
{
    k->repeat = repeat;
}

int KTFrame::repeat() const
{
    return k->repeat;
}

int KTFrame::index() const
{
    return layer()->visualIndexOf(const_cast<KTFrame *>(this));
}

KTLayer *KTFrame::layer() const
{
    return static_cast<KTLayer *>(parent());
}

KTScene *KTFrame::scene() const
{
    return layer()->scene();
}

KTProject *KTFrame::project() const
{
    return layer()->project();
}

int KTFrame::graphicItemsCount()
{
    return k->graphics.count();
}

int KTFrame::svgItemsCount()
{
    return k->svg.count();
}

int KTFrame::getTopZLevel()
{
    return k->graphics.count();
}

QList<int> KTFrame::itemIndexes()
{
    return k->graphics.indexes();
}

QList<int> KTFrame::svgIndexes()
{
    return k->svg.indexes();
}
