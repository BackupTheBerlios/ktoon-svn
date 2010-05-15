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


#include "kworkspacemainwindow.h"
#include <QWorkspace>

KWorkspaceMainWindow::KWorkspaceMainWindow(QWidget *parent) : KMainWindow(parent)
{
    m_workspace = new QWorkspace;
    setCentralWidget(m_workspace);
}

KWorkspaceMainWindow::~KWorkspaceMainWindow()
{
}

void KWorkspaceMainWindow::addWidget(QWidget *widget, int perspective)
{
    addToPerspective(widget, perspective);
    m_workspace->addWindow(widget);
    widget->show();
}

void KWorkspaceMainWindow::removeWidget(QWidget *widget)
{
    removeFromPerspective(widget);
    widget->setParent(0);
}
