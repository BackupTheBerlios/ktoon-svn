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

#ifndef SYMBOL_VIEW_H
#define SYMBOL_VIEW_H

/**
 * @file symbolview.h
 * @brief Include this file if you need the class SymbolView
 */

#include <qgl.h>

class GLGraphicComponent;
class KToon;

/**
 * @brief Class that handles the graphic symbol view into the library dialog box
 *
 * <b>Date of Creation: September 14 - 2004.</b>\n
 */
class SymbolView : public QGLWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a SymbolView object.
     * @param parent The parent widget
     * @param share The QGLWidget which the SymbolView is going to share display lists with
     */
    SymbolView( QWidget *parent, QGLWidget *share );
    /**
     * @brief Default Destructor
     *
     * Destroys the SymbolView object.
     */
    ~SymbolView();

    /**
     * @brief Gets this widget's share widget
     *
     * @return A pointer to the share widget
     */
    QGLWidget *shareWidget() const;
    /**
     * @brief Displays the graphic specified
     *
     * @param graphic The graphic to display
     */
    void display( GLGraphicComponent *graphic );

private:
    QWidget *parent_widget;
    QGLWidget *share_widget;
    GLGraphicComponent *displayed_graphic;
    bool dragging;
    bool valid_drag;
    KToon *k_toon;

signals:

public slots:

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
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse move control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseMoveEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse release control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseReleaseEvent( QMouseEvent *mouse_event );
    /**
     * @brief Draws a frame into this widget
     */
    void drawFrame();
    /**
     * @brief Shows a graphic when there is no selected symbol or a folder is selected
     */
    void drawDisabled();

};

#endif
