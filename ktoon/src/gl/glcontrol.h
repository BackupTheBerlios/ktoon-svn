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

#ifndef GLCONTROL_H
#define GLCONTROL_H

/*!
 * \file glcontrol.h
 * \brief Include this file if you need the class GLControl
 */

#include <qgl.h>
#include <qcolor.h>

class Ktoon;

/*!
 * \brief Class that has some drawing area's input events
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class implements some base events including mouse, wheel, tablet and keyboard events, for reimplement
 * into subclasses that need them.
 */
class GLControl : public QGLWidget
{

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a GLControl object.
     * \param parent The parent widget
     * \param grandparent The grandparent widget. It is used for slot connections or function calls
     * \param name The class name. It is passed to the QGLWidget constructor
     * \param share A widget for context sharing. It is passed to the QGLWidget constructor
     * \param f Flags passed to the QGLWidget constructor
     */
    GLControl( QWidget *parent, QWidget *grandparent, const char *name = 0, QGLWidget *share = 0, WFlags f = 0 );
    /*!
     * \brief Default Destructor
     *
     * Destroys the GLControl object.
     */
    ~GLControl();

    //Set methods
    /*!
     * \brief Sets a new point to the current tool's last position
     *
     * \param new_position The new position
     * \sa newPosition()
     */
    void setNewPosition( const QPoint & new_position );
    /*!
     * \brief Sets a new point to the current tool's first position
     *
     * \param old_position The old position
     * \sa oldPosition()
     */
    void setOldPosition( const QPoint & old_position );
    /*!
     * \brief Sets the wacom tablet pressure property
     *
     * \param pressure The new tablet pressure
     * \sa pressureTablet()
     */
    void setPressureTablet( const int & pressure );

    //Get methods
    /*!
     * \brief Gets the current tool's last position
     *
     * \return A point with the new position
     * \sa setNewPosition()
     */
    QPoint newPosition() const;
    /*!
     * \brief Gets the current tool's first position
     *
     * \return A point with the old position
     * \sa setOldPosition()
     */
    QPoint oldPosition() const;
    /*!
     * \brief Gets the current wacom tablet pressure property
     *
     * \return The tablet pressure
     * \sa setPressureTablet()
     */
    int pressureTablet() const;

protected:
    /*!
     * \brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget mouse release control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mouseReleaseEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget mouse move control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mouseMoveEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget mouse double click control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mouseDoubleClickEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget tablet event control
     *
     * Reimplemented from QWidget.
     * \param tablet_event The input event
     */
    void tabletEvent( QTabletEvent *tablet_event );

private:
    QPoint old_position;
    QPoint new_position;
    int pressure_tablet;
    QWidget *parent_widget, *grandparent_widget;
    Ktoon *k_toon;

};

#endif
