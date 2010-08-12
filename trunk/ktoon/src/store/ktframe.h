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

#ifndef KTFRAME_H
#define KTFRAME_H

#include <QGraphicsScene>
#include <QDomDocument>
#include <QDomElement>

#include "ktabstractserializable.h"
#include "ktinthash.h"
#include "ktsvgitem.h"
#include "ktglobal_store.h"

class KTFrame;
class KTLayer;
class KTGraphicObject;
class KTSvgItem;
class KTProject;
class KTScene;

typedef KTIntHash<KTGraphicObject *> GraphicObjects;
typedef KTIntHash<KTSvgItem *> SvgObjects;

/**
 * @brief Esta clase representa un marco o frame de la animacion
 * @author David Cuadrado \<krawek@toonka.com\>
*/
class STORE_EXPORT KTFrame : public QObject, public KTAbstractSerializable
{
    public:
       /**
        * Constructor por defecto
        */
       KTFrame(KTLayer *parent);
       
       /**
        * Destructor
        */
       ~KTFrame();
       
       /**
        * Pone el nombre del frame
        */
       void setFrameName(const QString &name);
       
       /**
        * Bloquea el frame
        */
       void setLocked(bool isLocked);
       
       /**
        * Retorna el nombre del frame
        */
       QString frameName() const;
       
       /**
        * Returna verdadero cuando el frame esta bloqueado
        */
       bool isLocked() const;
       
       void setVisible(bool isVisible);
       bool isVisible() const;
       
       void addItem(QString &id, QGraphicsItem *item);
       void addSvgItem(QString &id, KTSvgItem *item);
       void insertItem(int position, QString &id, QGraphicsItem *item);
       void insertSvgItem(int position, QString &id, KTSvgItem *item);
       
       void replaceItem(int position, QGraphicsItem *item);
       bool moveItem(int currentPosition, int newPosition);
       
       bool removeGraphicAt(int position);
       QGraphicsItem *createItem(int position, const QString &xml, bool loaded = false);
       KTSvgItem *createSvgItem(int position, const QString &xml, bool loaded = false);

       void setGraphics(GraphicObjects objects);       
       void setSvgObjects(SvgObjects objects);
       GraphicObjects graphics() const;
       SvgObjects svgItems() const; 
       
       KTGraphicObject *graphic(int position) const;
       KTSvgItem *svg(int position) const; 
       QGraphicsItem *item(int position) const;
       
       QGraphicsItemGroup *createItemGroupAt(int position, QList<qreal> group);
       QList<QGraphicsItem *> destroyItemGroup(int position);
             
       KTLayer *layer() const;
       KTScene *scene() const;
       KTProject *project() const;
       
       int indexOf(KTGraphicObject *object) const;
       int indexOf(QGraphicsItem *item) const;
       int indexOf(KTSvgItem *item) const;
       
       int index() const;
       
       void setRepeat(int repeat);
       int repeat() const;
       
       void clean();
       int graphicItemsCount();
       int svgItemsCount();

       int getTopZLevel();
       
    public:
       virtual void fromXml(const QString &xml);
       virtual QDomElement toXml(QDomDocument &doc) const;
       
    private:
       struct Private;
       Private *const k;
};

#endif
