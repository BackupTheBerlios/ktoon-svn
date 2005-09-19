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

#ifndef DRAWING_AREA_QT
#define DRAWING_AREA_QT

/**
 * @file drawingareaqt.h
 * @brief Include this file if you need the class DrawingAreaQt
 */

#include <qcanvas.h>

class QtGrid;
class KToon;

/**
 * @brief Class that handles the dialog box with an area where it is possible to make illustrations (For non graphic accelerated machines)
 *
 * <b>Date of Creation: Feb 28 - 2005.</b>\n
 * This is the most important widget of the illustration mode. Here, the user may create any type of drawing
 * with the help of the tools within the dialog boxes. It is equivalent to the class DrawingArea, except for
 * it is implemented by means of QCanvas instead of OpenGL. The reason is to provide a drawing area that
 * doesn't need to run into accelerated machines.
 */
class DrawingAreaQt : public QCanvasView
{
    Q_OBJECT
    
public:
    /**
     * @brief Default Constructor
     *
     * Constructs a DrawingAreaQt object
     * @param parent The parent widget
     * @param grandparent The grandparent widget
     * @param name The document name
     */
    DrawingAreaQt( QWidget *parent, const QString &name );
    /**
     * @brief Default Destructor
     *
     * Destroys the DrawingAreaQt object
     */
    ~DrawingAreaQt();

private:
    QWidget *parent_widget;
    QCanvas *_canvas;
    QtGrid *canvas_grid;
    KToon *k_toon;
    bool dragging;
    QCanvasItem *current_graphic;
    double max_z;
    QPoint moving_start;
    QCanvasRectangle *zoom_rectangle;
    QCanvasRectangle *selection_rectangle;
    bool drawing_pen, applying_bezier;

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QScrollView.
     * @param mouse_event The input event
     */
    void contentsMousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse move control
     *
     * Reimplemented from QScrollView.
     * @param mouse_event The input event
     */
    void contentsMouseMoveEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse release control
     *
     * Reimplemented from QScrollView.
     * @param mouse_event The input event
     */
    void contentsMouseReleaseEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse double click control
     *
     * Reimplemented from QScrollView.
     * @param mouse_event The input event
     */
    void contentsMouseDoubleClickEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for dialog box resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
    /**
     * @brief Event for dialog box painting control
     *
     * Reimplemented from QFrame.
     * @param paint_event The input event
     */
    void paintEvent( QPaintEvent *paint_event );
    /**
     * @brief Event for dialog box key press control
     *
     * Reimplemented from QWidget.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );
};

#endif
