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

#ifndef RENDER_CAMERA_PREVIEW_QT
#define RENDER_CAMERA_PREVIEW_QT

/**
 * @file rendercamerapreviewqt.h
 * @brief Include this file if you need the class RenderCameraPreviewQt
 */

#include <q3canvas.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>

/**
 * @brief Class that handles the dialog box that displays the render preview (For non graphic accelerated machines)
 *
 * <b>Date of Creation: Mar 9 - 2005.</b>\n
 * Equivalent to GlRenderCameraPreview implemented by means of OpenGL.
 */
class RenderCameraPreviewQt : public Q3CanvasView
{
    Q_OBJECT
    
public:
    /**
     * @brief Default Constructor
     *
     * Constructs a RenderCameraPreviewQt object
     * @param parent The parent widget
     * @param grandparent The grandparent widget
     * @param share The canvas to be shared
     */
    RenderCameraPreviewQt( QWidget *parent, QWidget *grandparent, Q3Canvas *share );
    /**
     * @brief Default Destructor
     *
     * Destroys the RenderCameraPreviewQt object
     */
    ~RenderCameraPreviewQt();

private:
    QWidget *parent_widget, *grandparent_widget;
    
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
