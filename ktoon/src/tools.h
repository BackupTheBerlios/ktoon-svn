/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#ifndef TOOLS_H
#define TOOLS_H

/*!
 * \file tools.h
 * \brief Include this file if you need the class Tools
 */

#include <qdialog.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qpixmap.h>

class Ktoon;

/*!
 * \brief Class that handles the dialog box where the special tools are found
 *
 * <b>Date of Creation: May 21 - 2004.</b>\n
 * Contains buttons and menu items where the user may choose a tool that he wants such as the brush, the pencil,
 * the eraser, etc.
 */
class Tools : public QDialog
{
    Q_OBJECT

public:
    //! Constants for defining tool types
    enum ToolType
    {
        NORMAL_SELECTION = 1, /*!< The Selection Tool */
        CONTOUR_SELECTION = 2, /*!< The Node Selection Tool */
        BRUSH = 3, /*!< The Brush Tool */
        PENCIL = 4, /*!< The Pencil Tool */
        PEN = 5, /*!< The Pen Tool */
        LINE = 6, /*!< The Line Tool */
        RECTANGLE = 7, /*!< The Rectangle Tool */
        ELLIPSE = 8, /*!< The Ellipse Tool */
        CONTOUR_FILL = 9, /*!< The Contour Fill Tool */
        FILL = 10, /*!< The Fill Tool */
        REMOVE_FILL = 11, /*!< The Remove Fill Tool */
        DROPPER = 12, /*!< The Dropper Tool */
        ERASER = 13, /*!< The Eraser Tool */
        SLICER = 14, /*!< The Slicer Tool */
        MAGNIFYING_GLASS = 15, /*!< The Magnifying Tool */
        HAND = 16 /*!< The Hand Tool */
    };

    /*!
     * \brief Default Constructor
     *
     * Constructs a Tools dialog box.
     * \param parent The parent widget
     * \param style Flags passed to the QDialog constructor
     * \param in_assigned_menu The assigned menu bar popup menu
     * \param id_assigned_item The assigned menu item from the assigned menu
     * \param assig_tb_button The assigned tool bar button
     */
    Tools( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button );
    /*!
     * \brief Default Destructor
     *
     * Destroys the Tools dialog box.
     */
    ~Tools();

    /*!
     * \brief Changes the button image according to what tool was selected
     *
     * \param id_button The selected tool
     */
    void changeButtonImage( int id_button );

private:
    //General purpose variables
    QWidget * parent_widget;
    QPopupMenu * assigned_menu;
    int assigned_item;
    QToolButton *assigned_tb_button;

    //Icons
    QPixmap icon_selection, icon_nodes, icon_brush, icon_pencil, icon_line, icon_pen, icon_rectangle,
            icon_ellipse, icon_eraser, icon_slicer, icon_fill, icon_contour_fill, icon_remove_fill,
            icon_dropper, icon_magnifying_glass, icon_hand;

    //Main Buttons
    QToolButton * selection_button;
    QToolButton * draw_button;
    QToolButton * fill_button;
    QToolButton * eraser_button;
    QToolButton * view_button;

    //Menus associated with every main button
    QPopupMenu * menu_button_selection;
    QPopupMenu * menu_button_draw;
    QPopupMenu * menu_button_fill;
    QPopupMenu * menu_button_eraser;
    QPopupMenu * menu_button_view;

protected:
    /*!
     * \brief Event for dialog box closing control
     *
     * It updates some main window features as the assigned menu item from the assigned menu.
     * Reimplemented from QWidget.
     * \param close_event The input event
     */
    void closeEvent( QCloseEvent *close_event );

};

#endif
