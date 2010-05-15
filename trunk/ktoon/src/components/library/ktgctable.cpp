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

#include "ktgctable.h"

#include <QHeaderView>

#include <kcore/kglobal.h>

/**
 * This class provides some methods for the KTLibraryWidget.
 * Here is where folder methods are defined for the Library widget.
 * @author David Cuadrado <krawek@toonka.com>
*/

KTGCTable::KTGCTable(QWidget *parent) : KTreeListWidget(parent), m_currentFolder(0)
{
    setHeaderLabels(QStringList() << "" << "" );
    header()->setResizeMode(QHeaderView::ResizeToContents);
}

KTGCTable::~KTGCTable()
{
}

void KTGCTable::createFolder(const QString &name)
{
    QTreeWidgetItem *newFolder = new QTreeWidgetItem(this);

    if (name.isNull())
        newFolder->setText(0, tr("New folder %1").arg(topLevelItemCount ()));
    else
        newFolder->setText(0, name);

    newFolder->setIcon(0, QPixmap(THEME_DIR + "icons/open.png"));
    m_currentFolder = newFolder;

    setCurrentItem(m_currentFolder);
}

QTreeWidgetItem *KTGCTable::currentFolder()
{
    return m_currentFolder;
}

void KTGCTable::setCurrentFolder(QTreeWidgetItem *cf)
{
    if (cf)
        m_currentFolder = cf;
}

void KTGCTable::removeCurrentFolder()
{
    if (m_currentFolder) {
        int index = indexOfTopLevelItem(m_currentFolder) - 1;

        delete m_currentFolder;

        m_currentFolder = topLevelItem (index);
        setCurrentItem(m_currentFolder);
    }
}

