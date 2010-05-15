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

#ifndef KSTACKEDMAINWINDOW_H
#define KSTACKEDMAINWINDOW_H

#include <kgui/kmainwindow.h>

#include <QHash>

class QStackedWidget;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_IDEAL_EXPORT KStackedMainWindow : public KMainWindow
{
    Q_OBJECT;
    public:
        KStackedMainWindow(QWidget *parent = 0);
        ~KStackedMainWindow();
        void addWidget(QWidget *widget, int perspective = DefaultPerspective);
        void removeWidget(QWidget *widget);
        
    private slots:
        void setupPerspective(int perspective);
        
    private:
        QStackedWidget *m_stack;
        QHash<int, QWidget *> m_widgets;
};

#endif
