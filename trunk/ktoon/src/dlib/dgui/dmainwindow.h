/***************************************************************************
 *   Copyright (C) 2006 by Alexander Dymo                                  *
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

#ifndef IDEALMAINWINDOW_H
#define IDEALMAINWINDOW_H

#include <QList>
#include <QMainWindow>

#include "didefs.h"
#include "dibuttontoolbar.h"

namespace Ideal {

class DiToolView;
class DiToolViewWidget;

/**
@short Main Window for the Ideal UI.
*/
class Q_GUI_EXPORT DMainWindow: public QMainWindow {
public:
    DMainWindow(QWidget *parent = 0);
    ~DMainWindow();
    
    /**Adds the toolview @p view to the main window. The toolview button will not be shown
    until the area is set for the mainwindow using @ref setArea method. The toolview
    itself will be shown as late as possible.
    @param defaultPlace defines where in the window the toolview will be placed (note that
    this parameter is only a hint to mainwindow areas to place the view for the first time).
    @param area is or-ed list of allowed areas for this toolview.*/
    virtual void addToolView(QWidget *view, Ideal::Place defaultPlace, bool persistant = false);
    /**Removes permanently the toolview from the main window. The current area is
    asked to remove it from the window layout also. To set just the visibility
    of the toolview use @ref hideToolView and @ref showToolView.*/
    virtual void removeToolView(QWidget *view);

    /** @return the list of all available toolviews in the mainwindow including
    hidden views and views not available in the current area.*/
    QList<DiToolView*> toolViews() const;
    /** @return the list of all toolviews in the given place in the
    mainwindow including hidden views and views not available in the current area.
    @param mode defines the or-ed list of toolview modes (DiToolView::Mode) - visible and/or enabled*/
    QList<DiToolView*> toolViews(Ideal::Place place, int mode) const;
    /** @return the button bar for given @p place.*/
    DiButtonToolBar *buttonBar(Ideal::Place place);

protected:
    /**Factory method to create the toolview. Reimplement this to return
    DiToolView subclasses here.*/
    virtual DiToolView *createToolView(QWidget *view, Ideal::Place defaultPlace, bool persistant = false);
    /**Factory method to create the button bar for toolview buttons.
    Reimplement this to return DiButtonToolBar subclasses here.*/
    virtual DiButtonToolBar *createButtonBar(Ideal::Place place);

private:
    struct MainWindowPrivate *d;
    friend class MainWindowPrivate;

};

}

#endif
