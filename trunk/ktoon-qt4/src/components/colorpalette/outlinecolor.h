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

#ifndef OUTLINECOLOR_H
#define OUTLINECOLOR_H

/**
 * @file outlinecolor.h
 * @brief Include this file if you need the class OutlineColor
 */

#include <q3frame.h>
#include <qpainter.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QMouseEvent>

/**
 * @brief Class that handles the color palette's outline color widget
 *
 * <b>Date of Creation: July 6 - 2004.</b>\n
 * This is a widget that shows the current outline color.
 */
class OutlineColor : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an OutlineColor object.
     * @param parent The parent widget
     */
    OutlineColor( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the OutlineColor object.
     */
    ~OutlineColor();

    /**
     * @brief Returns the active state of the OutlineColor
     *
     * @return <b>true</b> if the OutlineColor is active, otherwise returns <b>false</b>
     * @sa setActive()
     */
    bool isActive();
    /**
     * @brief Gets the current outline color
     *
     * @return The current color
     * @sa slotSetColor()
     */
    QColor currentColor();
    /**
     * @brief Gets the current outline color alpha
     *
     * @return The current alpha
     * @sa slotChangeAlpha()
     */
    short int getAlpha();
    /**
     * @brief Sets the active state of the OutlineColor
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
     * @brief Sets a new outline color and displays it in this widget
     *
     * @param new_color The new color
     * @sa currentColor()
     */
    void slotSetColor( const QColor &new_color );
    /**
     * @brief Sets a new alpha value to the current outline color and updates this widget's visualization
     *
     * @param new_alpha The alpha value (0..100)
     * @sa getAlpha()
     */
    void slotChangeAlpha( int new_alpha );

protected:
    /**
     * @brief Draw the contents of the OutlineColor
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
