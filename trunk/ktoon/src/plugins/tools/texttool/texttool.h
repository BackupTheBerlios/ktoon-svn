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
 
#ifndef ATEXTTOOL_H
#define ATEXTTOOL_H

#include <kttoolplugin.h>
#include <kttextitem.h>

#include "textconfigurator.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class TextTool : public KTToolPlugin
{
    Q_OBJECT;
    
    public:
        TextTool();
        virtual ~TextTool();
        
        virtual QStringList keys() const;
        
        virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void doubleClick(const KTInputDeviceInformation *input, KTGraphicsScene *scene);
        virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual bool itemPressed(QGraphicsItem *item);
        
        virtual QMap<QString, KAction *>actions() const;
        
        int toolType() const;
        
        virtual QWidget *configurator();
        
        void aboutToChangeScene(KTGraphicsScene *scene);
        void aboutToChangeTool();
        virtual void saveConfig();
        
    private:
        void setupActions();
        
    private:
        KTTextItem *m_item;
        TextConfigurator *m_configurator;
        QPoint m_position;
        QMap<QString, KAction *> m_actions;
};

#endif
