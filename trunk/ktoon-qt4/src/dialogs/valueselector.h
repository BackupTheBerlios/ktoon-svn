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

#ifndef VALUESELECTOR_H
#define VALUESELECTOR_H

/**
 * @file valueselector.h
 * @brief Include this file if you need the class ValueSelector
 */

#include <qwidget.h>
#include <q3rangecontrol.h>
#include <qpainter.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>

/**
 * @brief Class that handles the color palette's value selector
 *
 * <b>Date of Creation: July 7 - 2004.</b>\n
 * This widget manages the V (Value) component of the HSV color representation.
 */
class ValueSelector: public QWidget, public Q3RangeControl
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a ValueSelector object.
     * @param parent The parent widget
     */
    ValueSelector( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the ValueSelector object.
     */
    ~ValueSelector();

    /**
     * @brief Sets a new color and updates the visualization
     *
     * @param new_color The new color
     */
    void setColor( const QColor &new_color );

private:
    QWidget *parent_widget;
    QPainter painter;
    QColor highlight, shadow, triangle_color;
    QColor current_color;
    bool dragging;

signals:
    /**
     * This signal is emitted when the current value has changed.
     * @param value The value which the ValueSelector changed to
     */
    void valueChanged( int value );

public slots:
    /**
     * @brief Sets a new value to this widget
     *
     * @param new_value The new value
     */
    void slotSetValue( int new_value );

protected:
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
     * @brief Event for widget painting control
     *
     * Reimplemented from QWidget.
     * @param paint_event The input event
     */
    void paintEvent( QPaintEvent *paint_event );
    /**
     * @brief Gets the y axis point that matches the argument \a value
     *
     * @param value The value
     * @return The matching point
     */
    int pointFromValue( int value );
    /**
     * @brief Gets the value that matches the argument \a point
     *
     * @param point The y axis point
     * @return The matching value
     */
    int valueFromPoint( int point );

};

#endif
