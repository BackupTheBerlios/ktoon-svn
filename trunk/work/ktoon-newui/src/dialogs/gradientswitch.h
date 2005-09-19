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

#ifndef GRADIENTSWITCH_H
#define GRADIENTSWITCH_H

/**
 * @file gradientswitch.h
 * @brief Include this file if you need the class GradientSwitch
 */

#include <qwidget.h>
#include <qpainter.h>

/**
 * @brief Class that handles the Gradient Selector switches
 *
 * <b>Date of Creation: July 7 - 2004.</b>\n
 * This is a widget that contains a color and is used by the gradient selector to define the colors and the
 * position of each color of the gradient.
 */
class GradientSwitch: public QWidget
{
    Q_OBJECT

public:
    //! Constants for defining directions of a drag operation in a switch
    enum DragDirection
    {
        DRAG_LEFT = 500, /**< The left direction */
        DRAG_RIGHT = 501, /**< The right direction */
    };

    /**
     * @brief Default Constructor
     *
     * Constructs a GradientSwitch object.
     * @param parent The parent widget
     */
    GradientSwitch( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the GradientSwitch object.
     */
    ~GradientSwitch();

    /**
     * @brief Gets the position of the switch
     *
     * @return The switch position
     * @sa setPosition()
     */
    int position();
    /**
     * @brief Returns the select state of the switch
     *
     * @return <b>true</b> if the switch is selected, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isSelected();
    /**
     * @brief Gets the current color of the switch
     *
     * @return The current color
     * @sa slotSetColor()
     */
    QColor currentColor();
    /**
     * @brief Gets the color's alpha component of the switch
     *
     * @return The alpha component (0..100)
     * @sa slotSetAlpha()
     */
    int getAlpha();
    /**
     * @brief Sets the active state of the FillColor
     *
     * @param pos The boolean value of the active state
     * @sa position()
     */
    void setPosition( int pos );
    /**
     * @brief Sets the select state of the switch
     *
     * @param in_is_selected The boolean value of the select state
     * @sa isSelected()
     */
    void setSelected( bool in_is_selected );

private:
    QWidget *parent_widget;
    QPainter painter;
    QColor current_color;
    QColor border_color;
    int click_point;
    bool dragging;
    bool is_selected;
    int position_;
    int alpha;

signals:
    /**
     * This signal is emitted when this widget has been clicked.
     */
    void clicked();
    /**
     * This signal is emitted when this widget has been dragged.
     *
     * @param d The drag direction. See DragDirection
     */
    void dragged( int d );
    /**
     * This signal is emitted when the color of this switch has changed.
     *
     * @param c The new color
     */
    void colorChanged( const QColor &c );
    /**
     * This signal is emitted when the color's alpha component of this switch has changed
     *
     * @param a The new alpha
     */
    void alphaChanged( int a );
    /**
     * This signal is emitted when this widget has been deleted.
     */
    void deleted();

public slots:
    /**
     * @brief Sets a new color for this switch
     *
     * @param new_color The new color
     * @sa currentColor()
     */
    void slotSetColor( const QColor &new_color );
    /**
     * @brief Sets a new alpha for this switch
     *
     * @param new_alpha The new alpha
     * @sa getAlpha()
     */
    void slotSetAlpha( int new_alpha );

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

};

#endif
