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

#ifndef KTHELPWIDGET_H
#define KTHELPWIDGET_H

#include <QWidget>
#include <QMap>

#include <QTreeWidget>
#include <qdir.h>

#include "ktmodulewidgetbase.h"

class KHelpWidgetManager;

/**
 * @author David Alejandro Cuadrado Cabrera <krawek@gmail.com>
*/
class KTHelpWidget : public KTModuleWidgetBase
{
    Q_OBJECT
    public:
        KTHelpWidget(const QString &path, QWidget *parent = 0);
        ~KTHelpWidget();
        QString helpPath() const;

    private slots:
        void tryToLoadPage(QTreeWidgetItem *, int);
        void loadPage(const QString &path);

    signals:
        void pageLoaded(const QString &content);

    private:
        QDir m_helpPath;
        QMap<QTreeWidgetItem *, QString> m_files;
};

#endif
