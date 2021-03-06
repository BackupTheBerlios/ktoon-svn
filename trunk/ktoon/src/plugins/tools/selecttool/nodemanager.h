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
#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <QHash>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "node.h"


/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class NodeManager
{
    public:
        NodeManager(QGraphicsItem * parent, QGraphicsScene *scene);
        ~NodeManager();
        void syncNodes(const QRectF &sbr);
        void syncNodesFromParent();
        QGraphicsItem *parentItem() const;
        bool isModified() const;
        //  void setModify(bool modify);
        
        void beginToEdit();
        void restoreItem();
        
        void show();
        
        void setAnchor(const QPointF& point);
        QPointF anchor() const;
        
        void scale(float sx, float sy);
        void rotate(double a);
        
        void setPress(bool press);
        bool isPress();
        
        void toggleAction();
        
        void setActionNode(Node::ActionNode action);
        
        void setVisible(bool visible);
        
        double rotation();
        
    private:
        QHash<Node::TypeNode, Node *> m_nodes;
        QGraphicsItem * m_parent;
        
        QGraphicsScene *m_scene;
        
        // bool m_modify;
        
        QMatrix m_origMatrix;
        QPointF m_origPos;
        QPointF m_anchor;
        
        bool m_press;
        double m_rotation;

};

#endif
