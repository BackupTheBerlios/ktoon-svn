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
 
#ifndef KTLINEGUIDE_H
#define KTLINEGUIDE_H

#include <QGraphicsItem>
#include "ktglobal.h"

/**
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
*/
class KTOON_EXPORT KTLineGuide : public QGraphicsItem
{
    public:
        KTLineGuide(Qt::Orientation o, QGraphicsScene *scene);
        ~KTLineGuide();
        
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
        
        void setEnabledSyncCursor(bool enabled);
        
    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        
        // void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
        
        bool sceneEvent(QEvent *e);
        
    private:
        struct Private;
        Private * const k;
        void syncCursor();
};

#endif
