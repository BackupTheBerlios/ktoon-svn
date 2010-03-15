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

#ifndef KTSCENESLIST_H
#define KTSCENESLIST_H

#include <kgui/ktreelistwidget.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTScenesList : public KTreeListWidget
{
    Q_OBJECT
    public:
        KTScenesList(QWidget *parent = 0);
        ~KTScenesList();
        void addScene(const QString &name);
        void insertScene(int index, const QString &name);

        int removeCurrentScene();
        int indexCurrentScene();
        int scenesCount();

        QString nameCurrentScene();

        void removeScene(int index);
        void renameScene(int index, const QString &name);

        void selectScene(int index);

    private slots:
        void changeCurrentScene();
        void changeCurrentScene(QTreeWidgetItem *item, int c);

    public slots:
        int moveCurrentSceneUp();
        int moveCurrentSceneDown();
        void changeCurrentName(QString name);

    signals:
        void changeCurrent(QString name, int index);
};

#endif
