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

#ifndef FILLTOOLPLUGIN_H
#define FILLTOOLPLUGIN_H

#include <QObject>
#include <QLabel>

#include <kttoolplugin.h>

class KTPathItem;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class FillTool : public KTToolPlugin
{
    Q_OBJECT;
    
    public:
        FillTool();
        ~FillTool();
        
        virtual QStringList keys() const;

        void init(KTGraphicsScene *scene);
        
        virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, 
                    KTGraphicsScene *scene);

        virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, 
                    KTGraphicsScene *scene);

        virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, 
                    KTGraphicsScene *scene);
        
        KTPathItem *itemPressed(QGraphicsItem *item, const KTBrushManager *brush);
        
        virtual QMap<QString, KAction *> actions() const;
        
        int toolType() const;
        
        virtual QWidget *configurator();
        void aboutToChangeScene(KTGraphicsScene *scene);
        virtual void aboutToChangeTool();
        
        QPainterPath mapPath(const QPainterPath &path, const QPointF &pos);
        QPainterPath mapPath(const QGraphicsPathItem *item);

        virtual void saveConfig();
 
    private:
        void setupActions();
        
    private:
        QMap<QString, KAction *> m_actions;
};

#endif
