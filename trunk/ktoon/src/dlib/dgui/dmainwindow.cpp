/***************************************************************************
 *   Copyright (C) 2005-2006 by Alexander Dymo                             *
 *   adymo@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "dmainwindow.h"

#include <QMap>
#include <QLayout>

// #include "area.h"
#include "dibuttontoolbar.h"
#include "ditoolview.h"

namespace Ideal {

//==================== MainWindowPrivate =====================

struct MainWindowPrivate {
    MainWindowPrivate(DMainWindow *window)
        :w(window)
    {
    }
    ~MainWindowPrivate()
    {
    }

    DMainWindow *w;
    QList<DiToolView*> toolViews;
    QMap<QWidget*, DiToolView*> toolViewsForWidget;
    QMap<Ideal::Place, DiButtonToolBar*> buttonBars;
    
    void initButtonBar(Ideal::Place place);
};

void MainWindowPrivate::initButtonBar(Ideal::Place place)
{
    DiButtonToolBar *bar = w->createButtonBar(place);
    w->addToolBar(bar->toolBarPlace(), bar);
    buttonBars[place] = bar;
}



//======================== DMainWindow ========================

DMainWindow::DMainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    d = new MainWindowPrivate(this);

    d->initButtonBar(Ideal::Left);
    d->initButtonBar(Ideal::Right);
    d->initButtonBar(Ideal::Bottom);
    d->initButtonBar(Ideal::Top);

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
}

DMainWindow::~DMainWindow()
{
    delete d;
}

void DMainWindow::addToolView(QWidget *view, Ideal::Place defaultPlace, bool persistant)
{
    DiToolView *toolView = createToolView(view, defaultPlace, persistant);
    d->toolViews.append(toolView);
    d->toolViewsForWidget[view] = toolView;
    
//     toolView->setViewVisible( true );
}

void DMainWindow::removeToolView(QWidget *view)
{
    DiToolView *toolView = d->toolViewsForWidget[view];
    if (!toolView)
        return;

    d->toolViews.removeAll(toolView);
    d->toolViewsForWidget.remove(view);
    d->buttonBars[toolView->place()]->removeToolViewButton(toolView->button());
}

QList<DiToolView*> DMainWindow::toolViews() const
{
    return d->toolViews;
}

QList<DiToolView*> DMainWindow::toolViews(Ideal::Place place, int mode) const
{
    QList<DiToolView*> list;
    foreach (DiToolView *view, d->toolViews)
    {
        if (view->mode() & mode)
            list << view;
    }
    return list;
}

DiButtonToolBar *DMainWindow::buttonBar(Ideal::Place place)
{
    return d->buttonBars[place];
}

DiToolView *DMainWindow::createToolView(QWidget *view, Ideal::Place defaultPlace, bool persistant)
{
    return new DiToolView(this, view, defaultPlace, persistant);
}

DiButtonToolBar *DMainWindow::createButtonBar(Ideal::Place place)
{
    return new DiButtonToolBar(place, this);
}

}
