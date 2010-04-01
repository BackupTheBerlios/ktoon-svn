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

#include "ktframe.h"
#include "ktlayer.h"

#include <kcore/kdebug.h>

#include <QGraphicsItem>

#include "ktitemfactory.h"
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
    int repeat;
    //int zLevelBase;
};

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), k(new Private)
{
    k->name = "Frame";
    k->isLocked = false;
    k->isVisible = true;
    k->repeat = 1;
}

KTFrame::~KTFrame()
{
    k->graphics.clear(true);
    delete k;
}

void KTFrame::clean()
{
    k->graphics.clear(true);
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

                   KTGraphicObject *last = 0;

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

                              createItem(k->graphics.count(), newDoc);
                              last = k->graphics.visualValue(k->graphics.count()-1);
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

    foreach (KTGraphicObject *object, k->graphics.visualValues())
             root.appendChild(object->toXml(doc));

    return root;
}

void KTFrame::addItem(QGraphicsItem *item)
{
    insertItem(k->graphics.count(), item);
}

void KTFrame::insertItem(int position, QGraphicsItem *item)
{
    if (k->graphics.contains(position-1)) {
        if (QGraphicsItem *lastItem = k->graphics.visualValue(position-1)->item())
            item->setZValue(lastItem->zValue()+1);
    }

    KTGraphicObject *object = new KTGraphicObject(item, this);
    k->graphics.insert(position, object);
}

QGraphicsItemGroup *KTFrame::createItemGroupAt(int position, QList<qreal> group)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    qSort(group.begin(), group.end());

    KTItemGroup *g = new KTItemGroup(0);
    int count = 0;

    foreach (qreal p, group) {
             int pos = (int)p - count;
             QGraphicsItem *item = this->item(pos);
             KTGraphicObject *object = this->graphic(pos);
             object->setItem(0);
             removeGraphicAt(pos);
             g->addToGroup(item);
             delete object;
             count++;
    }

    insertItem(position, g);

    return g;
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
             double tmp = k->graphics.visualValue(i)->item()->zValue();
             k->graphics.visualValue(i)->item()->setZValue(k->graphics.visualValue(i+1)->item()->zValue());
             k->graphics.visualValue(i+1)->item()->setZValue(tmp);
             k->graphics.moveVisual(i, i+1);
        }
    } else {
             for (int i = currentPosition; i > newPosition; i--) {
                  double tmp = k->graphics.visualValue(i)->item()->zValue();
                  k->graphics.visualValue(i)->item()->setZValue(k->graphics.visualValue(i-1)->item()->zValue());
                  k->graphics.visualValue(i-1)->item()->setZValue(tmp);
                  k->graphics.moveVisual(i, i-1);
             }
    }

    return true;
}

bool KTFrame::removeGraphicAt(int position)
{
    if (position < 0 || position >= k->graphics.count())
        return false;

    KTGraphicObject *object = k->graphics.takeVisual(position);

    QGraphicsItem *item = object->item();

    if (item) {
        QGraphicsScene *scene = item->scene();

        if (scene)
            scene->removeItem(item);

        this->scene()->removeTweeningObject(object);

        return true;
    }

    return false;
}

QGraphicsItem *KTFrame::createItem(int position, const QString &xml, bool loaded)
{
    KTItemFactory itemFactory;
    itemFactory.setLibrary(project()->library());

    QGraphicsItem *item = itemFactory.create(xml);

    if (item)
        insertItem(position, item);

    if (loaded)
        KTProjectLoader::createItem(scene()->visualIndex(), layer()->visualIndex(), visualIndex(), position, xml, project());

    return item;
}

GraphicObjects KTFrame::graphics() const
{
    return k->graphics;
}

KTGraphicObject *KTFrame::graphic(int position) const
{
    if (position < 0 || position >= k->graphics.count()) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound (KTFrame->graphic())" << position;
        #endif

        return 0;
    }

    return k->graphics.visualValue(position);
}

QGraphicsItem *KTFrame::item(int position) const
{
    KTGraphicObject *object = graphic(position);

    if (object)
        return object->item();

    return 0;
}

int KTFrame::visualIndexOf(KTGraphicObject *object) const
{
    return k->graphics.visualIndex(object);
}

int KTFrame::logicalIndexOf(KTGraphicObject *object) const
{
    return k->graphics.logicalIndex(object);
}

int KTFrame::visualIndexOf(QGraphicsItem *item) const
{
    foreach (KTGraphicObject *object, k->graphics.values()) {
             if (object->item() == item)
                 return k->graphics.visualIndex(object);
    }

    return -1;
}

int KTFrame::logicalIndexOf(QGraphicsItem *item) const
{
    foreach (KTGraphicObject *object, k->graphics.values()) {
             if (object->item() == item)
                 return k->graphics.logicalIndex(object);
    }

    return -1;
}

int KTFrame::logicalIndex() const
{
    return layer()->visualIndexOf(const_cast<KTFrame *>(this));
}

void KTFrame::setRepeat(int repeat)
{
    k->repeat = repeat;
}

int KTFrame::repeat() const
{
    return k->repeat;
}

int KTFrame::visualIndex() const
{
    return layer()->logicalIndexOf(const_cast<KTFrame *>(this));
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

int KTFrame::count()
{
    return k->graphics.count();
}

/*
void KTFrame::setZLevel(int level)
{
    k->zLevelBase = level;
}

int KTFrame::getZLevel()
{
    return k->zLevelBase;
}
*/

int KTFrame::getTopZLevel()
{
    return k->graphics.count();
}
