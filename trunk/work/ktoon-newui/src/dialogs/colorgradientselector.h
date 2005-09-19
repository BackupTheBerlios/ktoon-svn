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

#ifndef COLORGRADIENTSELECTOR_H
#define COLORGRADIENTSELECTOR_H

/**
 * @file colorgradientselector.h
 * @brief Include this file if you need the class ColorGradientSelector
 */

#include <qcolor.h>
#include <qpainter.h>
#include <qptrlist.h>

#include "gradientswitch.h"

/**
 * @brief Class that handles the color palette's gradient selector
 *
 * <b>Date of Creation: July 7 - 2004.</b>\n
 * This is a widget where the user can edit a multicolor gradient with the help of gradient switches.
 */
class ColorGradientSelector: public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a ColorGradientSelector object.
     * @param parent The parent widget
     */
    ColorGradientSelector( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the ColorGradientSelector object.
     */
    ~ColorGradientSelector();

    /**
     * @brief Gets the current switch
     *
     * @return A pointer to the current switch
     */
    GradientSwitch *currentSwitch();
    /**
     * @brief Gets the switch that is before the argument switch
     *
     * @param gradient_switch The argument switch
     * @return A pointer to the switch that is before the argument. If there is no such, returns NULL
     * @sa switchAfter()
     */
    GradientSwitch *switchBefore( GradientSwitch *gradient_switch );
    /**
     * @brief Gets the switch that is after the argument switch
     *
     * @param gradient_switch The argument switch
     * @return A pointer to the switch that is after the argument. If there is no such, returns NULL
     * @sa switchBefore()
     */
    GradientSwitch *switchAfter( GradientSwitch *gradient_switch );

private:
    QWidget *parent_widget;
    QPainter painter;
    QColor highlight, shadow;
    QColor color_array[169];
    QPtrList<GradientSwitch> switches;
    GradientSwitch *fake_switch;
    GradientSwitch *current_switch;

signals:
    /**
     * This signal is emitted when any color of the gradient has changed.
     * @param color1 The first color
     * @param color2 The second color
     * @param pos_color1 The first color position within the widget
     * @param pos_color2 The second color position within the widget
     * @param alpha1 The first color alpha (0..100)
     * @param alpha2 The second color alpha (0..100)
     */
    void gradientChanged( const QColor &color1, const QColor &color2, int pos_color1, int pos_color2, int alpha1, int alpha2 );
    /**
     * This signal is emitted when a switch is selected.
     * @param color the current color of the selected switch
     */
    void switchSelected( const QColor &color );
    /**
     * This signal is emitted when a switch is selected.
     * @param alpha the current alpha of the selected switch
     */
    void switchSelected( int alpha );

public slots:
    /**
     * @brief Performs a switch drag and updates the gradient visualization
     *
     * @param direction The direction which the switch was dragged to
     * @sa slotSelectSwitch(), slotDeleteSwitch()
     */
    void slotDragSwitch( int direction );
    /**
     * @brief Performs a switch selection
     *
     * This slot is activated because of a mouse press event on a switch, so you never should
     * call it as a public member.
     * @sa slotDragSwitch(), slotDeleteSwitch()
     */
    void slotSelectSwitch();
    /**
     * @brief Updates the gradient visualization when a switch color or alpha was changed
     *
     * This slot is activated because of a switch emitted a signal, so you never should
     * call it as a public member.
     */
    void slotUpdateGradients();
    /**
     * @brief Performs a switch deletion
     *
     * This slot is activated because of a mouse press event on a switch, so you never should
     * call it as a public member.
     * @sa slotDragSwitch(), slotSelectSwitch()
     */
    void slotDeleteSwitch();

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget painting control
     *
     * Reimplemented from QWidget.
     * @param paint_event The input event
     */
    void paintEvent( QPaintEvent *paint_event );
    /**
     * @brief Computes the intermediate colors from \a color1 to \a color2 and updates the color array.
     *
     * This function generates \a pos_color1 - \a pos_color2 intermediate colors from \a color1 to \a color2,
     * that are combined in some proportion according to their position. The colors are set in the color array
     * from \a pos_color1 to \a pos_color2.
     * @param color1 The first color
     * @param color2 The second color
     * @param pos_color1 The position of the first color
     * @param pos_color2 The position of the second color
     */
    void interpolateColors( const QColor &color1, const QColor &color2, int pos_color1, int pos_color2 );

};

#endif
