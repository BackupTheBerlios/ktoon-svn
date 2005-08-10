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

#ifndef ESLAYER_H
#define ESLAYER_H

/**
 * @file eslayer.h
 * @brief Include this file if you need the class ESLayer
 */

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qpopupmenu.h>

/**
 * @brief Class that handles the Layer objects belonging to the Exposure Sheet dialog box
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * This is a widget that represents the exposure sheet layers as they are used in ToonBoom.
 */
class ESLayer : public QPushButton
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an ESLayer object.
     * @param initial_text The text that this object will display
     * @param parent The parent widget
     * @param grandparent The grandparent widget. It is used for slot connections or function calls
     */
    ESLayer( const QString &initial_text, QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the ColorCells object.
     */
    ~ESLayer();

    /**
     * @brief Returns the select state of the ESLayer
     *
     * @return <b>true</b> if the ESLayer is selected, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isSelected();
    /**
     * @brief Sets the select state of the ESLayer
     *
     * @param in_is_selected The boolean value of the select state
     * @sa isSelected()
     */
    void setSelected( bool in_is_selected );
    /**
     * @brief Clears the textfield focus on the layer
     *
     * This is a convenience function that clears explicitly the focus of the textfield "description"
     * in order to emit the signal lostFocus(). You won't need to use it.
     */
    void clearTextfieldFocus();

private:
    QColor default_color, selection_color;
    QLineEdit *description;
    QPopupMenu *right_click_menu;
    bool is_selected;

signals:
    /**
     * This signal is emitted when the ESLayer is renamed.
     * @param n The new name
     */
    void renamed( const QString &n );

public slots:
    /**
     * @brief Sets the textfield text that appears when someone double-clicks the layer, to this frame's description.
     *
     * You should never use it from outside.
     */
    void slotSetDescription();
    /**
     * @brief Sends the double click event in order to the right click menu may call the rename routine
     *
     * You should never use it from ouside.
     */
    void slotSendDoubleClickEvent();

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse double click control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseDoubleClickEvent( QMouseEvent *mouse_event );
	      void resizeEvent ( QResizeEvent * e );
};

#endif
