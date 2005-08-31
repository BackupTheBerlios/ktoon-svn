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

#ifndef TLLAYERSEQUENCE_H
#define TLLAYERSEQUENCE_H

/**
 * @file tllayersequence.h
 * @brief Include this file if you need the class TLLayerSequence
 */

#include <qscrollview.h>
#include <qptrlist.h>

#include "tllayer.h"

class Layer;
class KToon;

/**
 * @brief Class that handles the Sequence of Layers into the Layer Manager
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * A layer sequence manages multiple layers into a scrollview.
 */
class TLLayerSequence : public QScrollView
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a TLLayerSequence object.
     * @param parent The parent widget
     */
    TLLayerSequence( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLLayerManager object.
     */
    ~TLLayerSequence();

    /**
     * @brief Gets the list of layers
     *
     * @return The list of layers
     */
    QPtrList<TLLayer> listOfLayers();
    /**
     * @brief Gets the number of layers
     *
     * @return The number of layers
     */
    int numberOfLayers();
    /**
     * @brief Gets the current layer
     *
     * @return A pointer to the current layer
     */
    TLLayer *currentLayer();
    /**
     * @brief Gets the last layer
     *
     * @return A pointer to the last layer (The lowermost)
     */
    TLLayer *lastLayer();
    /**
     * @brief Gets a layer at the specified position
     *
     * @param pos The 1-based position
     * @return A pointer to the layer in that position
     */
    TLLayer *layerAt( int pos );
    /**
     * @brief Sets all layers to the state "no dragged"
     */
    void clearDraggedLayers();
    /**
     * @brief Loads the specified layers
     *
     * @param layers The list of layers
     */
    void loadLayers( QPtrList<Layer> layers );
    /**
     * @brief Sets whether is being performed a drag operation within this widget
     *
     * @param drag The dragging state
     */
    void setDragging( bool drag );

private:
    //General Purpose Variables
    QWidget *parent_widget;
    int number_of_layers;
    int layer_max_value;
    bool dragging;
    TLLayer *dragged_layer;
    TLLayer *current_layer;
    TLLayer *last_layer;
    QPtrList<TLLayer> list_of_layers;
    KToon *k_toon;

signals:
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
     * This signal is emitted when a layer has been selected. It is used for synchronizing with exposure sheet.
     * @param pos The selected layer position
     */
    void layerSelectedToES( int pos );
    /**
     * This signal is emitted when a layer has been renamed.
     * @param pos The position of the renamed layer
     * @param name The new name
     */
    void layerRenamed( int pos, const QString &name );
    /**
     * This signal is emitted when a layer has changed its visibility state.
     * @param pos The position of the changed layer
     * @param state The visibility state
     */
    void layerVisibilityChanged( int pos, bool state );

public slots:
    /**
     * @brief Performs a layer selection when a frame is selected
     *
     * @param frame_sequence The current frame sequence
     */
    void slotSelectLayerFromFrameSelected( TLFrameSequence *frame_sequence );
    /**
     * @brief Inserts a new layer
     */
    void slotInsertLayer();
    /**
     * @brief Removes the current layer
     */
    void slotRemoveLayer();
    /**
     * @brief Performs a layer selection
     *
     * Updates the layer colors and emits some signals.
     */
    void slotSelectLayer();
    /**
     * @brief Slot that is useful because emits the layerRenamed() signal
     *
     * Do not use it as a public member.
     * @param name The new name
     */
    void slotRenameLayer( const QString &name );
    /**
     * @brief Slot that is useful because emits the layerVisibilityChanged() signal
     *
     * Do not use it as a public member.
     * @param state The visibility state
     */
    void slotChangeVisibilityState( bool state );
    /**
     * @brief Performs a layer selection when a layer in exposure sheet is selected
     *
     * @param pos The layer position
     */
    void slotSelectLayerFromES( int pos );
    /**
     * @brief Renames a layer when a layer in exposure sheet has been renamed.
     *
     * @param pos The layer position
     * @param name The new name
     */
    void slotRenameLayerFromES( int pos, const QString &name );
    /**
     * @brief Moves the current layer one step to the top
     *
     * @sa slotMoveLayerDown()
     */
    void slotMoveLayerUp();
    /**
     * @brief Moves the current layer one step to the bottom
     *
     * @sa slotMoveLayerUp()
     */
    void slotMoveLayerDown();
    /**
     * @brief Performs a layer drag action to the top
     *
     * @param y The y axis drag point
     * @sa slotDragLayerBelow()
     */
    void slotDragLayerAbove( int y );
    /**
     * @brief Performs a layer drag action to the bottom
     *
     * @param y The y axis drag point
     * @sa slotDragLayerAbove()
     */
    void slotDragLayerBelow( int y );
    /**
     * @brief Performs a layer release action to the top
     *
     * @param y The y axis release point
     * @sa slotReleaseLayerBelow()
     */
    void slotReleaseLayerAbove( int y );
    /**
     * @brief Performs a layer release action to the bottom
     *
     * @param y The y axis release point
     * @sa slotReleaseLayerAbove()
     */
    void slotReleaseLayerBelow( int y );

protected:
    /**
     * @brief Event for widget resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
    /**
     * @brief Shows scroll bars if the contents are outside the viewport
     */
    void updateContentSize();
    /**
     * @brief Gets the layer position associated with the y axis point
     *
     * @return The layer position
     */
    int mapToPosition( int y );

};

#endif
