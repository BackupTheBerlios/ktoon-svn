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
 
#ifndef TWEENERTRANSLATE_H
#define TWEENERTRANSLATE_H

#include <kttoolplugin.h>

/**
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
 * experimental plugin
*/

class Tweener : public KTToolPlugin
{
    Q_OBJECT;
    public:

        Tweener();
        virtual ~Tweener();
        virtual void init(KTGraphicsScene *scene);

        virtual QStringList keys() const;
        virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
        virtual void updateScene(KTGraphicsScene *scene);

        virtual QMap<QString, KAction *>actions() const;

        int toolType() const;

        virtual QWidget *configurator();

        void aboutToChangeScene(KTGraphicsScene *scene);

        virtual void aboutToChangeTool();

        bool isComplete() const;
        virtual void saveConfig();

    private:
        void setupActions();
        QString pathToCoords();

    private:
        struct Private;
        Private *const k;

    public slots:
        void setCreatePath();
        void setSelect();
        void applyTweener();
        void updatePath();
};

#endif
