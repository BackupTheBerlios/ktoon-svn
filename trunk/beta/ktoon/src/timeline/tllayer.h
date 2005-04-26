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

#ifndef TLLAYER_H
#define TLLAYER_H

/**
 * @file tllayer.h
 * @brief Include this file if you need the class TLLayer
 */

#include <qlabel.h>
#include <qcheckbox.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include <qcolor.h>
#include <qpopupmenu.h>

#include "tlframesequencemanager.h"

class TLLayerSequence;
class TLLayerManager;

/**
 * @brief Class that handles the Timeline's Layer representation
 *
 * <b>Date of Creation: June 11 - 2004.</b>\n
 * This layer representation is really near to the Flash's layer representation.
 */
class TLLayer : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a TLLayer object.
     * @param in_position The position of this layer
     * @param parent The parent widget
     * @param grandparent The grandparent widget. It is used for slot connections or function calls
     * @param initial_name The initial name that this layer will display
     */
    TLLayer( int in_position, QWidget *parent, QWidget *grandparent, const QString &initial_name );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLLayer object.
     */
    ~TLLayer();

    /**
     * @brief Gets this layer's position
     *
     * @return The position of the layer
     * @sa setPosition()
     */
    int position();
    /**
     * @brief Gets this layer's name
     *
     * @return The current name
     * @sa setName()
     */
    QString name();
    /**
     * @brief Returns the edition state of the TLLayer
     *
     * @return <b>true</b> if the TLLayer is being edited, otherwise returns <b>false</b>
     * @sa setEdited()
     */
    bool isBeingEdited();
    /**
     * @brief Returns the visibility state of the TLLayer
     *
     * @return <b>true</b> if the TLLayer is visible, otherwise returns <b>false</b>
     * @sa setVisible()
     */
    bool isVisible();
    /**
     * @brief Returns the lock state of the TLLayer
     *
     * @return <b>true</b> if the TLLayer is locked, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isLocked();
    /**
     * @brief Returns the select state of the TLLayer
     *
     * @return <b>true</b> if the TLLayer is selected, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isSelected();
    /**
     * @brief Returns the drag way state of the TLLayer
     *
     * A TLLayer is in drag way if the mouse pointer reaches it while draggin another layer. It is distinguished
     * by its lighter color.
     * @return <b>true</b> if the TLLayer is in drag way, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isInDragWay();
    /**
     * @brief Sets a new position to this frame sequence
     *
     * @param in_position The new position
     * @sa position()
     */
    void setPosition( int in_position );
    /**
     * @brief Sets a new name to this layer
     *
     * @param new_name The new name
     * @sa name()
     */
    void setName( const QString &new_name );
    /**
     * @brief Sets the edition state of the TLLayer
     *
     * @param in_is_being_edited The boolean value of the edition state
     * @sa isBeingEdited()
     */
    void setEdited( bool in_is_being_edited );
    /**
     * @brief Sets the visibility state of the TLLayer
     *
     * @param in_is_visible The boolean value of the visibility state
     * @sa isVisible()
     */
    void setVisible( bool in_is_visible );
    /**
     * @brief Sets the lock state of the TLLayer
     *
     * @param in_is_locked The boolean value of the lock state
     * @sa isLocked()
     */
    void setLocked( bool in_is_locked );
    /**
     * @brief Sets the select state of the TLLayer
     *
     * @param in_is_selected The boolean value of the select state
     * @sa isSelected()
     */
    void setSelected( bool in_is_selected );
    /**
     * @brief Sets the drag way state of the TLLayer
     *
     * A TLLayer is in drag way if the mouse pointer reaches it while draggin another layer. It is distinguished
     * by its lighter color.
     * @param in_is_in_drag_way The boolean value of the drag way state
     * @sa isInDragWay()
     */
    void setInDragWay( bool in_is_in_drag_way );
    /**
     * @brief Clears the textfield focus on the layer
     *
     * This is a convenience function that clears explicitly the focus of the textfield "edit_name"
     * in order to emit the signal lostFocus(). You won't need to use it.
     */
    void clearTextfieldFocus();
    /**
     * @brief Makes the layer to show only outlines or contours in all its objects
     *
     * @param outline_state The boolean value of the outline state
     */
    void seeOnlyOutlines( bool outline_state );

private:
    //General purpose variables
    QWidget *parent_widget, *grandparent_widget;
    int position_;
    bool is_being_edited;
    bool is_visible;
    bool is_locked;
    bool is_selected;
    bool is_in_drag_way;
    bool dragging;
    QColor default_color, selection_color, text_default_color, text_selection_color, drag_color;
    QPopupMenu *right_click_menu;

    //Icons
    QPixmap i_static_layer_image, i_edition_image, i_enabled, i_disabled;

    //Main Components
    QLabel *static_layer_image;
    QLabel *layer_name;
    QLabel *edition_image;
    QLabel *visibility_image;
    QLabel *lock_image;
    QCheckBox *only_outlines;
    QLineEdit *edit_name;

signals:
    /**
     * This signal is emitted when a layer is selected.
     */
    void selected();
    /**
     * This signal is emitted when a layer has been dragged to a bottom position.
     * @param pos The y coordinate of the drag movement
     */
    void draggedBelow( int pos );
    /**
     * This signal is emitted when a layer has been dragged to a top position.
     * @param pos The y coordinate of the drag movement
     */
    void draggedAbove( int pos );
    /**
     * This signal is emitted when a layer has been released to a bottom position.
     * @param pos The y coordinate of the release movement
     */
    void releasedBelow( int pos );
    /**
     * This signal is emitted when a layer has been released to a top position.
     * @param pos The y coordinate of the release movement
     */
    void releasedAbove( int pos );
    /**
     * This signal is emitted when a layer has been renamed.
     * @param name The new name
     */
    void renamed( const QString &name );
    /**
     * This signal is emitted when the visibility state changes
     * @param state The visibility state
     */
    void visibilityChanged( bool state );

public slots:
    /**
     * @brief A slot that is activated when the user changes the layer's name
     */
    void slotEditName();
    /**
     * @brief A convenience slot created because the checkbox does not respond to the mouse press event
     *
     * This slot usefulness is only to connect the check signal of the outline checkbox to set this layer to edition
     * and selection state. You should never use it.
     */
    void slotOnlyOutlines();
    /**
     * @brief Sends the double click event in order to the right click menu may call the rename routine
     *
     * You should never use it from ouside.
     */
    void slotSendDoubleClickEvent();

protected:
    /**
     * @brief Event for widget resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
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
     * @brief Event for widget mouse double click control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseDoubleClickEvent( QMouseEvent *mouse_event );

};

#endif
