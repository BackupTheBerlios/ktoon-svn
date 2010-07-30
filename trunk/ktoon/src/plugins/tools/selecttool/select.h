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

#ifndef SELECT_H
#define SELECT_H

#include <QObject>
#include <kttoolplugin.h>

#include "ktpathitem.h"
#include "ktproject.h"
#include "ktgraphicsscene.h"

class KTItemResponse;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class NodeManager;
class Select : public KTToolPlugin
{
    Q_OBJECT;
    
    public:
        Select();
        virtual ~Select();
        
        virtual void init(KTGraphicsScene *scene);
        virtual QStringList keys() const;
        virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void keyPressEvent(QKeyEvent *event);
        
        virtual QMap<QString, KAction *>actions() const;
        
        int toolType() const;
        
        virtual QWidget *configurator();
        
        virtual void aboutToChangeTool();
        void aboutToChangeScene(KTGraphicsScene *scene);
        
        virtual void itemResponse(const KTItemResponse *event);
        virtual void saveConfig();

    //signals:
    //    void itemAddedOnSelection();

    private:
        void setupActions();
        
    private:
        struct Private;
        Private *const k;
        
    private slots:
        void syncNodes();
        void updateItems(KTGraphicsScene *);
};

#endif