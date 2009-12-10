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

#include "ktscenetabwidget.h"
#include <QWheelEvent>
#include <QTabBar>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

#include <kcore/kdebug.h>

KTSceneTabWidget::KTSceneTabWidget(QWidget *parent) : QFrame(parent)
{
   QVBoxLayout *layout = new QVBoxLayout(this);
   layout->setMargin(1);

   //QLabel *header = new QLabel("Foreground -> Background");
   //layout->addWidget(header);

   tabber = new QTabWidget;
   layout->addWidget(tabber);

   setLayout(layout);
}

KTSceneTabWidget::~KTSceneTabWidget()
{
}

void KTSceneTabWidget::removeAllTabs()
{
    int count = tabber->count();
    for (int i = 0; i < count; i++)
         delete tabber->currentWidget();
}

void KTSceneTabWidget::addScene(int index, const QString &name, KTExposureTable *table) {

    QFrame *frame = new QFrame;
    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->setMargin(1);

    QLabel *header = new QLabel("Foreground -> Background");
    layout->addWidget(header);
    layout->addWidget(table);
    frame->setLayout(layout);

    tables.insert(index, table);
    tabber->insertTab(index, frame, name);
}

KTExposureTable* KTSceneTabWidget::getCurrentTable() 
{
    int index = currentIndex();
    /*
    KTExposureTable *table = tables.value(index);
    if (table) 
        return table;

    return 0;
    */
  
    return getTable(index);
}

KTExposureTable* KTSceneTabWidget::getTable(int index)
{
    KTExposureTable *table = tables.value(index);

    if (table)
        return table;

    return 0;
}

int KTSceneTabWidget::currentIndex()
{
    int index = tabber->currentIndex();
    return index;
}

void KTSceneTabWidget::setCurrentIndex(int index) 
{
    tabber->setCurrentIndex(index);
}

void KTSceneTabWidget::setTabText(int index, const QString &name)
{
    tabber->setTabText(index, name);
}

void KTSceneTabWidget::removeTab(int index)
{
    tabber->removeTab(index);
}

int KTSceneTabWidget::count()
{
    return tabber->count();
}

/*
#ifndef QT_NO_WHEELEVENT
void KTSceneTabWidget::wheelEvent(QWheelEvent *ev)
{
    QRect rect = tabBar()->rect();
    rect.setWidth(width());

    if (rect.contains(ev->pos()))
        wheelMove(ev->delta());
}

void KTSceneTabWidget::wheelMove( int delta )
{
    if (count() > 1) {
        int current = currentIndex();
        if (delta < 0) {
            current = (current + 1) % count();
        } else {
            current--;
            if (current < 0)
                current = count() - 1;
        }
        setCurrentIndex(current);
    }
}

#endif
*/
