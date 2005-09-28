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

#ifndef FILLCOLOR_H
#define FILLCOLOR_H

/**
 * @file fillcolor.h
 * @brief Include this file if you need the class FillColor
 */

#include <QFrame>
#include <qpainter.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QMouseEvent>

/**
 * @brief Class that handles the color palette's fill color widget
 *
 * <b>Date of Creation: July 6 - 2004.</b>\n
 * This is a widget that shows the current fill color.
 */
class FillColor : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a FillColor object.
     * @param parent The parent widget
     */
    FillColor( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the FillColor object.
     */
    ~FillColor();

    /**
     * @brief Returns the active state of the FillColor
     *
     * @return <b>true</b> if the FillColor is active, otherwise returns <b>false</b>
     * @sa setActive()
     */
    bool isActive();
    /**
     * @brief Gets the current fill color
     *
     * @return The current color
     * @sa slotSetColor()
     */
    QColor currentColor();
    /**
     * @brief Gets the current fill color alpha
     *
     * @return The current alpha
     * @sa slotChangeAlpha()
     */
    short int getAlpha();
    /**
     * @brief Sets the active state of the FillColor
     *
     * @param state The boolean value of the active state
     * @sa isActive()
     */
    void setActive( bool state );

private:
    QWidget *parent_widget;
    QPixmap icon;
    bool active;
    QColor current_color;
    QColor square_border_color, dark_color, light_color;
    short int alpha;

signals:
    /**
     * This signal is emitted when this widget has been activated.
     */
    void activated();

public slots:
    /**
     * @brief Sets a new fill color and displays it in this widget
     *
     * @param new_color The new color
     * @sa currentColor()
     */
    void slotSetColor( const QColor &new_color );
    /**
     * @brief Sets a new alpha value to the current fill color and updates this widget's visualization
     *
     * @param new_alpha The alpha value (0..100)
     * @sa getAlpha()
     */
    void slotChangeAlpha( int new_alpha );

protected:
    /**
     * @brief Draw the contents of the FillColor
     *
     * Reimplemented from QFrame.
     * @param painter The frame painter
     */
    void drawContents( QPainter *painter );
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );

};

#endif
