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

#ifndef TLFRAMESEQUENCELAYOUT_H
#define TLFRAMESEQUENCELAYOUT_H

/*!
 * \file tlframesequencelayout.h
 * \brief Include this file if you need the class TLFrameSequenceLayout
 */

#include <qscrollview.h>

#include "tlframesequence.h"

class TLLayer;
class Ktoon;
class Layer;
class KeyFrame;

/*!
 * \brief Class that handles the Timeline's Frame Sequence Layout
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * A frame sequence layout manages multiple frame sequences into a scrollview.
 */
class TLFrameSequenceLayout : public QScrollView
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a TLFrameSequenceLayout object.
     * \param parent The parent widget
     */
    TLFrameSequenceLayout( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the TLFrameSequenceLayout object.
     */
    ~TLFrameSequenceLayout();

    /*!
     * \brief Moves the frames of all sequences horizontally to the pixel denoted by \a x
     *
     * It is used for example when the horizontal scroll bar is moved
     * \param x The pixel that the frames are going to be moved
     */
    void moveFrames( int x );
    /*!
     * \brief Gets all the frame sequences
     *
     * \return A list with all the frame sequences
     */
    QPtrList<TLFrameSequence> listOfFrameSequences();
    /*!
     * \brief Gets the current frame
     *
     * \return A pointer to the current selected frame. NULL if there is no selected frame
     */
    TLFrame *currentFrame();
    /*!
     * \brief Gets the current frame sequence
     *
     * \return A pointer to the current frame sequence
     */
    TLFrameSequence *currentFrameSequence();
     /*!
      * \brief Loads keyframes from a list of layers
      *
      * \param layers The list of layers. They also provide the keyframe information
      */
     void loadFrames( QPtrList<Layer> layers );

private:
    QWidget *parent_widget;
    int number_of_frame_sequences;
    int max_used_frames;
    QPtrList<TLFrameSequence> list_of_frame_sequences;
    TLFrame *current_frame;
    TLFrameSequence *current_frame_sequence, *last_frame_sequence;
    Ktoon *k_toon;

signals:
    /*!
     * This signal is emitted when a frame is selected.
     * \param frame_sequence The frame sequence that corresponds to the selected frame
     */
    void frameSelected( TLFrameSequence *frame_sequence );
    /*!
     * This signal is emitted when the maximum number of used frames has changed.
     * \param max_used_frames The new maximun number of used frames
     */
    void maxUsedFramesUpdated( int max_used_frames );
    /*!
     * This signal is only for a bridge connection to ExposureSheet slots from TLFrameSequence.
     * \param key The position of the keyframe within the frame sequence that emitted the signal
     */
    void keyframeRemovedToES( int key );
    /*!
     * This signal is only for a bridge connection to ExposureSheet slots from TLFrameSequence.
     * \param key The position of the keyframe within the frame sequence that emitted the signal
     */
    void motionTweenCreatedToES( int key );
    /*!
     * This signal is only for a bridge connection to ExposureSheet slots from TLFrameSequence.
     * \param key The position of the keyframe within the frame sequence that emitted the signal
     */
    void motionTweenRemovedToES( int key );

public slots:
    /*!
     * \brief Performs all tasks related to all frame sequences when a layer is selected
     *
     * It changes the current frame sequence to be the frame sequence that matches the layer position.
     * \param selected_layer The selected layer
     */
    void slotConsequencesOfALayerSelection( TLLayer *selected_layer );
    /*!
     * \brief Sets the offset of all frame sequences to be the current offset
     *
     * \param current_offset The current offset
     */
    void slotChangeOffset( int current_offset );
    /*!
     * \brief Sets the drag offset of all frame sequences to be the current offset
     *
     * \param current_offset The current drag offset
     */
    void slotChangeDragOffset( int current_offset );
    /*!
     * \brief Performs a swap between the frame sequences at positions \a c_pos and \a r_pos
     *
     * \param c_pos The position of the first frame sequence
     * \param r_pos The position of the second frame sequence
     */
    void slotSwapFrameSequences( int c_pos, int r_pos );
    /*!
     * \brief Inserts keyframes into the current frame sequence
     *
     * \param number The number of keyframes to be inserted
     */
    void slotInsertKeyframesIntoTheCurrentFS( int number );
    /*!
     * \brief Removes a keyframe block from the current frame sequence
     *
     * See TLFrameSequence::slotRemoveKeyframeBlock().
     * \param key The keyframe position
     */
    void slotRemoveKeyframeBlockFromTheCurrentFS( int key );
    /*!
     * \brief Moves a keyframe block in the current frame sequence to the left
     *
     * See TLFrameSequence::slotMoveKeyframeLeft().
     * \param key The keyframe position
     */
    void slotMoveKeyframeLeftInTheCurrentFS( int key );
    /*!
     * \brief Moves a keyframe block in the current frame sequence to the right
     *
     * See TLFrameSequence::slotMoveKeyframeRight().
     * \param key The keyframe position
     */
    void slotMoveKeyframeRightInTheCurrentFS( int key );
    /*!
     * \brief Performs a frame selection
     */
    void slotSelectFrame();
    /*!
     * \brief Inserts a new frame sequence
     */
    void slotInsertFrameSequence();
    /*!
     * \brief Removes the current frame sequence
     */
    void slotRemoveFrameSequence();
    /*!
     * \brief Updates the value of the \e max_used_frames variable. Called after a frame insertion or deletion
     */
    void slotUpdateMaxUsedFrames();
    /*!
     * \brief Moves the current frame sequence one step to the top
     */
    void slotMoveFrameSequenceUp();
    /*!
     * \brief Moves the current frame sequence one step to the bottom
     */
    void slotMoveFrameSequenceDown();
    /*!
     * \brief Updates the render preview if a layer has changed its visibility state
     *
     * \param layer_pos The layer position
     * \param state The visibility state
     */
    void slotChangeLayerVisibility( int layer_pos, bool state );
    /*!
     * \brief Sets the current keyframe property of "has_drawing"
     *
     * \param state The boolean value of the "has_drawing" property
     */
    void slotFrameHasDrawing( bool state );

protected:
    /*!
     * \brief Event for widget resizing control
     *
     * Reimplemented from QWidget.
     * \param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );
    /*!
     * \brief Shows scroll bars if the contents are outside the viewport
     */
    void updateContentSize();

};

#endif
