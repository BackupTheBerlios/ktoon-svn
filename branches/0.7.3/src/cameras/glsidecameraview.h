/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#ifndef GLSIDECAMERAVIEW_H
#define GLSIDECAMERAVIEW_H

/**
 * @file glsidecameraview.h
 * @brief Include this file if you need the class GLSideCameraView
 */

#include <qpopupmenu.h>
#include <qgl.h>
#include <qtoolbutton.h>

/**
 * @brief Class that handles the dialog box for displaying the side camera view
 *
 * <b>Date of Creation: June 18 - 2004.</b>\n
 */
class GLSideCameraView : public QGLWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a GLSideCameraView object
     * @param parent The parent widget
     * @param grandparent The grandparent widget
     * @param in_assigned_menu The assigned menu bar popup menu
     * @param id_assigned_item The assigned menu item from the assigned menu
     * @param assig_tb_button The assigned tool bar button
     * @param share The object which this widget is going to share display lists with
     * @param f Flags passed to the DrawingArea constructor
     */
    GLSideCameraView( QWidget *parent, QWidget *grandparent, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button, QGLWidget *share = 0, WFlags f = 0 );
     /**
      * @brief Default destructor
      *
      * Destroys the GLSideCameraView object
      */
    ~GLSideCameraView();

protected:
    /**
     * @brief Initializes the OpenGL rendering context
     *
     * For a detailed description see the QGLWidget documentation.
     */
    void initializeGL();
    /**
     * @brief This function is called whenever this widget has been resized
     *
     * For a detailed description see the QGLWidget documentation.
     * @param w The new width
     * @param h The new height
     */
    void resizeGL( int w, int h );
    /**
     * @brief This function is called whenever this widget needs to be painted
     *
     * For a detailed description see the QGLWidget documentation.
     */
    void paintGL();
    /**
     * @brief Draws a frame into this widget
     */
    void drawFrame();
    /**
     * @brief Event for dialog box mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for dialog box mouse move control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseMoveEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for dialog box mouse release control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseReleaseEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for dialog box closing control
     *
     * Reimplemented from QWidget.
     * @param close_event The input event
     */
    void closeEvent( QCloseEvent *close_event );

private:
    //General Purpose Variables
    QWidget *parent_widget, *grandparent_widget;
    QGLWidget *share_widget;
    QPopupMenu *assigned_menu;
    int assigned_item;
    QToolButton *assigned_tb_button;
    GLuint max_vertical;
    GLuint max_horizontal;

};

#endif
