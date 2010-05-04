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

#ifndef KTSCENETABWIDGET_H
#define KTSCENETABWIDGET_H

#include <kcore/kglobal.h>
#include "ktexposuretable.h"

#include <QTabWidget>
#include <QHash>
#include <QLabel>
#include <QFrame>

/**
 * @author Xtingray 
*/
class K_GUI_EXPORT KTSceneTabWidget : public QFrame
{
    Q_OBJECT;
    public:
        KTSceneTabWidget(QWidget *parent = 0);
        ~KTSceneTabWidget();
        QTabWidget* TabWidget();
        void addScene(int index, const QString &name, KTExposureTable *table = 0);
        KTExposureTable* getCurrentTable();
        KTExposureTable* getTable(int index);
        int currentIndex();
        int count();

    private:
        QHash<int, KTExposureTable *> tables;
        QTabWidget *tabber;

    public slots:
        void removeAllTabs();
};

#endif
