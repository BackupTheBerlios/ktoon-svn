/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
 *                                                                         *
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

#ifndef KACTIONMANAGER_H
#define KACTIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QHash>

#include "kgui/kaction.h"
#include "kcore/kglobal.h"

typedef QList<QAction *> KActionList;
typedef QHash<QString, QAction *> KActionDict;
typedef QHash<QString, KActionDict> KActionContainer;

class QToolBar;
class QMenuBar;

/**
 * @short la clase DActionManager provee de un manejador de acciones, este manejador facilita el acceso y ordenamiento a las acciones contieniendo todas las acciones de la aplicacion.
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class K_GUI_EXPORT KActionManager : public QObject
{
    Q_OBJECT

    public:
        KActionManager(QObject *parent = 0);
        ~KActionManager();

        bool insert(QAction *action, const QString &id, const QString &container = "default");
        void remove(QAction* action, const QString &container = QString());

        QAction *take(QAction* action, const QString &container = QString());
        QAction *find(const QString &id, const QString &container = QString()) const;
        QAction *operator[](const QString &id) const;
        void enable(const QString &id, bool flag);

        QMenuBar *setupMenuBar(QMenuBar *menu, const QStringList &containers, bool clear = true);
        QMenu *setupMenu(QMenu *menu, const QString &container, bool clear = true);
        QToolBar *setupToolBar(QToolBar *toolBar, const QString &container, bool clear = true );

    private:
        KActionContainer m_actionContainer;
};

#endif
