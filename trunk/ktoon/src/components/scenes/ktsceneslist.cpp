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

#include "ktsceneslist.h"
#include <kcore/kdebug.h>

#include <QTreeWidgetItem>

// TODO: Add support for renaming and moving objects from the list

KTScenesList::KTScenesList(QWidget *parent) : KTreeListWidget(parent)
{
    setHeaderLabels(QStringList() << "name");
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(changeCurrentScene()));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(changeCurrentScene(QTreeWidgetItem *, int)));
}

KTScenesList::~KTScenesList()
{
}

void KTScenesList::addScene(const QString &name)
{
    QTreeWidgetItem *newScene = new QTreeWidgetItem(this);
    newScene->setText(0, name);
}

void KTScenesList::insertScene(int index, const QString &name)
{
    QTreeWidgetItem *newScene = new QTreeWidgetItem;
    newScene->setText(0, name);
    insertTopLevelItem(index, newScene);
}

void KTScenesList::changeCurrentName(QString name)
{
    currentItem()->setText(0, name);
} 

int KTScenesList::removeCurrentScene()
{
    int index = indexCurrentScene();

    if (currentItem()) {
        delete currentItem();
    }

    return index;
}

void KTScenesList::removeScene(int index)
{
    delete topLevelItem(index);
}

void KTScenesList::renameScene(int index, const QString &name)
{
    QTreeWidgetItem *item = topLevelItem(index);

    if (item)
        item->setText(0,name);
}

void KTScenesList::selectScene(int index)
{
    QTreeWidgetItem *item = topLevelItem(index);

    if (item)
        setCurrentItem(item);
}

void KTScenesList::changeCurrentScene()
{
    QString name = currentItem()->text(0);
    int index = indexCurrentScene();
    emit(changeCurrent(name, index));
}

void KTScenesList::changeCurrentScene(QTreeWidgetItem *item, int c)
{
    Q_UNUSED(c);
    QString name = item->text(0);
    int index = indexOfTopLevelItem(item);
    emit(changeCurrent(name, index));
}

int KTScenesList::moveCurrentSceneUp()
{
    int index = indexCurrentScene();
    if (index > 0)
        insertTopLevelItem(index-1, takeTopLevelItem(index));

    return index;
}

int KTScenesList::moveCurrentSceneDown()
{
    int index = indexCurrentScene();
    if (index < topLevelItemCount()-1) 
        insertTopLevelItem(index+1, takeTopLevelItem(index));

    return index;
}

int KTScenesList::indexCurrentScene()
{
    return indexOfTopLevelItem(currentItem());
}

QString KTScenesList::nameCurrentScene()
{
    return currentItem()->text(0);
}

int KTScenesList::scenesCount()
{
    return topLevelItemCount();
}
