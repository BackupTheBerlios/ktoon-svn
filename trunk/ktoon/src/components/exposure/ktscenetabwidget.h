/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2009 by Xtingray                                        *
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
        void addScene(int index, const QString &name, KTExposureTable *table = 0);
        KTExposureTable* getCurrentTable();
        KTExposureTable* getTable(int index);
        int currentIndex();
        void setCurrentIndex(int index);
        void setTabText(int index, const QString &name);
        void removeTab(int index);
        int count(); 
        QTabWidget *tabber;

    private:
        QHash<int, KTExposureTable *> tables;

    public slots:
        void removeAllTabs();

/*
    protected:
        #ifndef QT_NO_WHEELEVENT
                virtual void wheelEvent(QWheelEvent *e);
        #endif

    protected slots:
        #ifndef QT_NO_WHEELEVENT
                virtual void wheelMove(int delta);
        #endif
*/
};

#endif
