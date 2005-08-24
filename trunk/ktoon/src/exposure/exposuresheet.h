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

#ifndef EXPOSURESHEET_H
#define EXPOSURESHEET_H

/**
 * @file exposuresheet.h
 * @brief Include this file if you need the class ExposureSheet
 */

#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qptrlist.h>
#include <qtoolbutton.h>
#include <qvbox.h>
#include <qhbox.h>


#include "ktdialogbase.h"
#include "ilayer.h"

class KToon;
class Timeline;
class Layer;
class KeyFrame;
class GLDrawing;

/**
 * @brief Class that handles the exposure sheet dialog box
 *
 * <b>Date of Creation: June 4 - 2004.</b>\n
 * This is a class that manages the exposure sheet as it is used in ToonBoom. It shows basically a list of layers,
 * each one with its list of keyframes. It provides also some buttons for operating the components.
 */
class ExposureSheet : public KTDialogBase
{
    Q_OBJECT

public:
    //! Constants for defining exposure sheet properties
    enum ESProperties
    {
	MAX_NUMBER_OF_FRAMES = 100 /**< Constant that defines the maximum number of keyframes that will be created */
    };

    /**
     * @brief Default Constructor
     *
     * Constructs a EsposureSheet dialog box.
     * @param parent The parent widget
     * @param style Flags passed to the QDialog constructor
     * @param in_assigned_menu The assigned menu bar popup menu
     * @param id_assigned_item The assigned menu item from the assigned menu
     * @param assig_tb_button The assigned tool bar button
     */
      ExposureSheet( QWidget *parent);
    /**
     * @brief Default Destructor
     *
     * Destroys the ExposureSheet dialog box.
     */
    ~ExposureSheet();

    /**
     * @brief Gets the current layer
     *
     * @return A pointer to the current layer
     */
    ESLayer *currentLayer();
    /**
     * @brief Gets the current layer object
     *
     * @return A pointer to the ILayer object associated to the current layer
     */
    ILayer *currentLayerObj();
    /**
     * @brief Gets the exposure sheet layers
     *
     * @return A list with the exposure sheet layers
     */
    QPtrList<ESLayer> getLayers();
    /**
     * @brief Gets the exposure sheet layer objects
     *
     * The layer object is of type ILayer, instead of the interface layer that is called ESLayer.
     * @return A list with the exposure sheet layer objects
     */
    QPtrList<ILayer> getILayers();
    /**
     * @brief A function only for set the first frame of the first layer as the current status keyframe
     */
    void touchFirstFrame();
    /**
     * @brief A function only for set the first layer as the current status layer
     */
    void touchFirstLayer();
    /**
     * @brief Loads the specified layers and keyframes
     *
     * @param layers The list of layers. They also provide the keyframe information
     */
    void loadLayersAndKeyframes( QPtrList<Layer> layers );
    /**
     * @brief Gets the visible layers
     *
     * The visible layers are those that are checked into the visibility list, after showing it with the "eye" button.
     * @return A list of type Layer with the visible layers
     */
    QPtrList<Layer> visibleLayers();

private:
    int layer_max_value;
    int number_of_layers;
    QString current_name;
    ESLayer *first_layer;
    ESLayer *last_layer;
    ESLayer *current_layer;
    ESFrame *f_frame;
    KToon *k_toon;
    GLDrawing *to_copy;

    //Icons
    QPixmap i_insert_layer, i_remove_layer, i_lock_frame, i_insert_frame, i_remove_frame, i_move_frame_up,
            i_move_frame_down, i_layer_visibility;

    //Buttons
    QPushButton *insert_layer, *remove_layer, *lock_frame, *insert_frame, *remove_frame, *move_frame_up,
                *move_frame_down, *layer_visibility;

    //List of Layers
    QPtrList<ILayer> list_of_layers;

    //Scroll Area
    QScrollView *scroll_area_container;
    QFrame *scroll_area;

    //Widget for handling the layer visibility
    QListView *visibility_list;

signals:
    /**
     * This signal is emitted when a layer is inserted into the exposure sheet.
     */
    void layerInserted();
    /**
     * This signal is emitted when a layer is removed from the exposure sheet.
     */
    void layerRemoved();
    /**
     * This signal is emitted when a layer is selected.
     * @param pos The position of the selected layer (>= 1)
     */
    void layerSelected( int pos );
    /**
     * This signal is emitted when a layer is renamed.
     * @param pos The position of the renamed layer (>= 1)
     * @param name The new name
     */
    void layerRenamed( int pos, const QString &name );
    /**
     * This signal is emitted when frames are inserted at the end of the current layer.
     * @param number The number of insertions
     */
    void framesInsertedAtTheEnd( int number );
    /**
     * This signal is emitted when a frame is removed from the current layer.
     * @param pos The position of the removed frame (>= 1)
     */
    void frameRemoved( int pos );
    /**
     * This signal is emitted when a frame is moved up into the current layer.
     * @param pos The position of the moved frame (>= 1)
     */
    void frameMovedUp( int pos );
    /**
     * This signal is emitted when a frame is moved down into the current layer.
     * @param pos The position of the moved frame (>= 1)
     */
    void frameMovedDown( int pos );
    /**
     * This signal is emitted when a frame is selected.
     */
    void frameSelected();

public slots:
    /**
     * @brief Inserts a layer into the exposure sheet
     */
    void slotInsertLayer();
    /**
     * @brief Removes a layer from the exposure sheet
     */
    void slotRemoveLayer();
    /**
     * @brief Performs a layer selection
     *
     * This slot is activated because of a mouse press event on a layer, so you never should
     * call it as a public member.
     * @sa slotSelectLayerFromTL()
     */
    void slotSelectLayer();
    /**
     * @brief Performs a layer selection
     *
     * This was mainly intended for synchronization with the Timeline, so it was created with that part of the
     * name "FromTL". However, you may use it outside as a public member since it provides an useful argument for
     * select a layer knowing its position.
     * @param pos The layer position
     * @sa slotSelectLayer()
     */
    void slotSelectLayerFromTL( int pos );
    /**
     * @brief Renames the current layer
     *
     * @param name The new name
     * @sa slotRenameLayerFromTL()
     */
    void slotRenameLayer( const QString &name );
    /**
     * @brief Renames a layer
     *
     * This was mainly intended for synchronization with the Timeline, so it was created with that part of the
     * name "FromTL". However, you may use it outside as a public member since it provides an useful argument for
     * rename a layer knowing its position.
     * @param pos The layer position
     * @param name The new name
     * @sa slotRenameLayer()
     */
    void slotRenameLayerFromTL( int pos, const QString &name );
    /**
     * @brief Moves the current layer to the left
     *
     * @sa slotMoveLayerRight()
     */
    void slotMoveLayerLeft();
    /**
     * @brief Moves the current layer to the right
     *
     * @sa slotMoveLayerLeft()
     */
    void slotMoveLayerRight();
    /**
     * @brief Swaps a layer with another that is at its left side
     *
     * This function was created in order to synchronize the timeline drag layer operation. The "rel" part of
     * the second argument name refers to the "release layer" operation after a drag. You are allowed to use
     * it in any context.
     * @param cur_pos The position of the current layer
     * @param rel_pos The position of the layer that the current is going to swap with.
     * @pre rel_pos < cur_pos
     * @sa slotSwapWithRightLayer()
     */
    void slotSwapWithLeftLayer( int cur_pos, int rel_pos );
    /**
     * @brief Swaps a layer with another that is at its righ side
     *
     * This function was created in order to synchronize the timeline drag layer operation. The "rel" part of
     * the second argument name refers to the "release layer" operation after a drag. You are allowed to use
     * it in any context.
     * @param cur_pos The position of the current layer
     * @param rel_pos The position of the layer that the current is going to swap with.
     * @pre rel_pos > cur_pos
     * @sa slotSwapWithLeftLayer()
     */
    void slotSwapWithRightLayer( int cur_pos, int rel_pos );
    /**
     * @brief Locks the current frame
     */
    void slotLockFrame();
    /**
     * @brief Inserts a frame into the current layer
     *
     * If no frame is selected or is selected an used frame, it inserts a single frame at the end of the current
     * layer. If an unused frame is selected, it inserts frames until that frame position.
     * @note Inserting a frame is a logical operation since it does not inserts physically the frame object.
     * @note There are a fixed number of frames so this operation only sets used the frames inserted.
     */
    void slotInsertFrame();
    /**
     * @brief Removes a frame from the current layer
     *
     * If no frame is selected or is selected an unused frame, it removes a single frame at the end of the current
     * layer. If an used frame is selected, it removes that frame.
     * @note Removing a frame is a logical operation since it does not removes physically the frame object.
     * @note There are a fixed number of frames so this operation only sets unused the frames removed.
     */
    void slotRemoveFrame();
    /**
     * @brief Performs a frame selection
     *
     * This slot is activated because of a mouse press event on a frame, so you never should
     * call it as a public member.
     */
    void slotSelectFrame();
    /**
     * @brief Renames the current frame
     *
     * It updates the KeyFrame object related to the current frame.
     * @param new_name The frame's new name
     */
    void slotRenameFrame( const QString &new_name );
    /**
     * @brief Moves up the current frame within the current layer
     *
     * @sa slotMoveFrameDown()
     */
    void slotMoveFrameUp();
    /**
     * @brief Moves down the current frame within the current layer
     *
     * @sa slotMoveFrameUp()
     */
    void slotMoveFrameDown();
    /**
     * @brief Shows or hides a widget where it is possible to set the layers visible or not visible for the light table
     *
     * @param show The boolean value of the show property
     */
    void slotLayerVisibility( bool show );
    /**
     * @brief Removes a frame from the current layer
     *
     * @param key The frame position
     * @note Removing a frame is a logical operation since it does not removes physically the frame object.
     * @note There are a fixed number of frames so this operation only sets unused the frames removed.
     */
    void slotRemoveFrameFromTL( int key );
    /**
     * @brief Sets a frame to be a motion tween starter frame
     *
     * @param key The frame position
     * @sa slotRemoveMotionTween()
     */
    void slotCreateMotionTween( int key );
    /**
     * @brief Sets a frame not to be a motion tween starter frame
     *
     * @param key The frame position
     * @sa slotCreateMotionTween()
     */
    void slotRemoveMotionTween( int key );
    /**
     * @brief Stores the frame information to be pasted later
     */
    void slotCopyFrame();
    /**
     * @brief Pastes the frame that was previously copied
     */
    void slotPasteFrame();
    /**
     * @brief Updates the offset and length indicators for every ESFrame from the argument layer
     *
     * The offset and length indicators for an ESFrame are included within a QToolTip.
     * @param layer The layer that will be updated
     */
    void updateIndicators( ILayer *layer );

protected:
    /**
     * @brief Event for dialog box closing control
     *
     * It updates some main window features as the assigned menu item from the assigned menu.
     * Reimplemented from QWidget.
     * @param close_event The input event
     */
//     void closeEvent( QCloseEvent *close_event );
    /**
     * @brief Event for dialog box resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
    /**
     * @brief Event for dialog box key pressing control
     *
     * Reimplemented from QWidget.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );
    /**
     * @brief Gets the layer associated with a frame
     *
     * @param f The ESFrame object
     * @return The associated ILayer object
     */
    ILayer *associatedLayer( ESFrame *f );
    /**
     * @brief Gets the current layer object
     *
     * @return The ILayer associated with the selected ESLayer
     */
    ILayer *findCurrentLayerObj();
    /**
     * @brief Selects the keyframe that is inmediately up from the current keyframe
     *
     * If no keyframe is selected or there is no more space to the top, this function does nothing.
     */
    void selectUp();
    /**
     * @brief Selects the keyframe that is inmediately down from the current keyframe
     *
     * If no keyframe is selected or there is no more space to the bottom, this function does nothing.
     */
    void selectDown();
    /**
     * @brief Selects the keyframe that is inmediately right from the current keyframe
     *
     * If no keyframe is selected or there is no more space to the right, this function does nothing.
     */
    void selectRight();
    /**
     * @brief Selects the keyframe that is inmediately left from the current keyframe
     *
     * If no keyframe is selected or there is no more space to the left, this function does nothing.
     */
    void selectLeft();

};

#endif
