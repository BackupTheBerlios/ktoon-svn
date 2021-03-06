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

#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>
#include <QGraphicsScene>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KNodeGroup;
class KControlNode : public QObject, public QGraphicsItem
{
    Q_OBJECT;
    
    public:
        
        KControlNode(int index, KNodeGroup *nodeGroup, const QPointF & pos = QPoint(0,0),  
                     QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
        
        ~KControlNode();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
        QRectF boundingRect() const;
        enum { Type = UserType + 100 };
        
        int type() const { return Type; }
        
        void setLeft(KControlNode *left);
        void setRight(KControlNode *right);
        void setCentralNode(KControlNode *centralNode);
        int index() const;
        
        void setGraphicParent(QGraphicsItem * newParent);
        QGraphicsItem *graphicParent();
        
        KControlNode *left();
        KControlNode *right();
        KControlNode *centralNode();
        
        void hasChanged(bool notChange);
        
    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
     
    private:
        void paintLinesToChildNodes(QPainter * painter);
        
    public slots:
        void showChildNodes(bool visible);
        void setSeletedChilds(bool select);
        
    signals:
        void showBrothers(bool show);
        
    private:
        struct Private;
        Private *const k;
};

#endif
