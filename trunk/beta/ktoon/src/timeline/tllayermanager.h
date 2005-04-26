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

#ifndef TLLAYERMANAGER_H
#define TLLAYERMANAGER_H

/**
 * @file tllayermanager.h
 * @brief Include this file if you need the class TLLayerManager
 */

#include <qlabel.h>
#include <qpushbutton.h>

#include "tllayersequence.h"

/**
 * @brief Class that handles the Timeline's Layer Manager
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * This layer manager contains the layer sequence and buttons to perform operations such as add and remove layers,
 * lock or unlock layers, show or hide layers and move layers up and down.
 */
class TLLayerManager : public QWidget
{
    Q_OBJECT

public:
    //! Constants for defining layer states
    enum LayerStates
    {
        EYE_SELECT_ALL = 1000, /**< The next operation on the eye button will show all layers*/
        EYE_DESELECT_ALL = 1001, /**< The next operation on the eye button will hide all layers*/
        LOCK_ALL = 2000, /**< The next operation on the lock button will lock all layers*/
        UNLOCK_ALL = 2001, /**< The next operation on the lock button will unlock all layers*/
        SQUARE_SELECT_ALL = 3000, /**< The next operation on the square button will set all layers to show only outlines*/
        SQUARE_DESELECT_ALL = 3001 /**< The next operation on the square button will set all layers to show outlines and fills*/
    };

    /**
     * @brief Default Constructor
     *
     * Constructs a TLLayerManager object.
     * @param parent The parent widget
     * @param grandparent The grandparent widget. It is used for slot connections or function calls
     */
    TLLayerManager( QWidget *parent, QWidget *grandparent );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLLayerManager object.
     */
    ~TLLayerManager();

    /**
     * @brief Gets the layer sequence's vertical scroll bar
     *
     * @return A pointer to the layer sequence's vertical scroll bar
     */
    QScrollBar *verticalScrollBar();
    /**
     * @brief Gets the layer sequence
     *
     * @return A pointer to the layer sequence
     */
    TLLayerSequence *layerSequence();
    /**
     * @brief Gets the insert layer button
     *
     * @return A pointer to the insert layer button
     */
    QPushButton *insertLayerButton();
    /**
     * @brief Gets the remove layer button
     *
     * @return A pointer to the remove layer button
     */
    QPushButton *removeLayerButton();
    /**
     * @brief Sets all layer visible or invisible into the render preview
     *
     * @param visible The boolean value of the visibility state
     */
    void setAllLayersVisible( bool visible );

private:
    //General Purpose Variables
    QWidget *parent_widget, *grandparent_widget;
    int eye_status;
    int lock_status;
    int square_status;
    double current_time, total_time;

    //Icons
    QPixmap i_eye, i_lock, i_square, i_insert_layer, i_remove_layer, i_move_layer_up, i_move_layer_down;

    //Panel with eye, lock and square
    QFrame *utils;
    QPushButton *eye, *lock, *square;

    //Panel for inserting, removing, and moving layers
    QFrame *utils2;
    QPushButton *insert_layer, *remove_layer, *move_layer_up, *move_layer_down;
    QLabel *time;

    //The Layer Sequence!!
    TLLayerSequence *layer_sequence;

signals:
    /**
     * This signal is emitted when a frame is selected.
     * @param frame_sequence The frame sequence that corresponds to the selected frame
     */
    void frameSelected( TLFrameSequence *frame_sequence );
    /**
     * This signal is emitted when a layer has been selected.
     * @param layer The selected layer
     */
    void layerSelected( TLLayer *layer );
    /**
     * This signal is emitted when a layer has been released above its original position after a drag.
     * @param pos1 The position the layer had first
     * @param pos2 The release position
     */
    void layerReleasedAbove( int pos1, int pos2 );
    /**
     * This signal is emitted when a layer has been released below its original position after a drag.
     * @param pos1 The position the layer had first
     * @param pos2 The release position
     */
    void layerReleasedBelow( int pos1, int pos2 );
    /**
     * This signal is emitted when this widget's width has changed
     * @param width The new width
     */
    void widthChanged( int width );

public slots:
    /**
     * @brief Sets a new total animation time
     *
     * @param new_time The new animation time
     */
    void slotSetTotalTime( float new_time );
    /**
     * @brief Sets the new current offset time
     *
     * @param new_time The new current offset time
     */
    void slotSetCurrentTime( float new_time );

protected slots:
    /**
     * @brief A slot that is activated when the user presses the eye button. It's for show/hide all layers
     */
    void slotEye();
    /**
     * @brief A slot that is activated when the user presses the lock button. It's for lock/unlock all layers
     */
    void slotLock();
    /**
     * @brief A slot that is activated when the user presses the square button. It's for see only outlines on
     * all layers
     */
    void slotSquare();

protected:
    /**
     * @brief Event for widget resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );

};

#endif
