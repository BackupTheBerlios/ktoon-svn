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

#ifndef TLFRAMESEQUENCE_H
#define TLFRAMESEQUENCE_H

/**
 * @file tlframesequence.h
 * @brief Include this file if you need the class TLFrameSequence
 */

#include <qptrlist.h>

#include "tlframe.h"

class TLFrameSequenceLayout;
class TLRuler;
class TLFrameSequenceManager;
class Ktoon;
class KeyFrame;

/**
 * @brief Class that handles the Timeline's Frame Sequences
 *
 * <b>Date of Creation: June 22 - 2004.</b>\n
 * A frame sequence contains a fixed number of frames, denoted by the only constant of this class.
 */
class TLFrameSequence : public QWidget
{
    Q_OBJECT

public:
    //! Constants for defining frame sequence's properties
    enum FSProperties
    {
	TL_MAX_NUMBER_OF_FRAMES = 300 /**< Constant that defines the maximum number of frames that will be created */
    };

    /**
     * @brief Default Constructor
     *
     * Constructs a TLFrameSequence object.
     * @param in_position The position of this frame sequence
     * @param parent The parent widget
     * @param grandparent The grandparent widget. It is used for slot connections or function calls
     */
    TLFrameSequence( int in_position, QWidget *parent, QWidget *grandparent );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLFrameSequence object.
     */
    ~TLFrameSequence();

    /**
     * @brief Gets this frame sequence's position
     *
     * @return The position of the frame sequence
     * @sa setPosition()
     */
    int position();
    /**
     * @brief Gets the number of used frames within this frame sequence
     *
     * @return The number of used frames. See TLFrame for an explanation of what is an used frame
     */
    int numberOfUsedFrames();
    /**
     * @brief Gets the number of key frames within this frame sequence
     *
     * @return The number of key frames. See TLFrame for an explanation of what is a key frame
     */
    int numberOfKeyframes();
    /**
     * @brief Sets a new position to this frame sequence
     *
     * @param in_position The new position
     * @sa position()
     */
    void setPosition( int in_position );
    /**
     * @brief Sets a frame to be the frame sequence's offset frame
     *
     * See TLFrame for an explanation of what is an offset frame.
     * @param new_offset The position within the frame sequence of the frame that is going to be the offset.
     */
    void changeOffsetFrame( int new_offset );
    /**
     * @brief Sets a frame to be the frame sequence's drag offset frame
     *
     * See TLFrame for an explanation of what is a drag offset frame.
     * @param new_offset The position within the frame sequence of the frame that is going to be the drag offset.
     */
    void changeDragOffsetFrame( int new_offset );
    /**
     * @brief Loads the specified keyframes
     *
     * @param keyframes The list of keyframes
     */
    void loadFrames( QPtrList<KeyFrame> keyframes );
    /**
     * @brief Finds the previous keyframe relative to the argument frame
     *
     * @param frame The frame from which the keyframe will be searched
     * @return A pointer to the found keyframe. NULL if there is no keyframe before the argument frame
     * @sa findNextKeyframe()
     */
    TLFrame *findPrevKeyframe( TLFrame *frame );
    /**
     * @brief Finds the next keyframe relative to the argument frame
     *
     * @param frame The frame from which the keyframe will be searched
     * @return A pointer to the found keyframe. NULL if there is no keyframe after the argument frame
     * @sa findNextKeyframe()
     */
    TLFrame *findNextKeyframe( TLFrame *frame );
    /**
     * @brief Gets the position of the keyframe associated to the argument frame
     *
     * @param frame The frame from which the keyframe position will be searched
     * @return The keyframe postion
     */
    int keyframePosition( TLFrame *frame );
    /**
     * @brief Gets the position of the specified frame
     *
     * @param frame The frame which position will be searched
     * @return The frame postion
     */
    int framePosition( TLFrame *frame );
    /**
     * @brief Gets the current offset frame
     *
     * @return A pointer to the current offset frame
     */
    TLFrame *offsetFrame();
    /**
     * @brief Sets a keyframe property of "has_drawing"
     *
     * @param pos The keyframe position
     * @param state The boolean value of the "has_drawing" property
     */
    void setKeyframeHasDrawing( int pos, bool state );

private:
    QWidget *parent_widget, *grandparent_widget;
    int position_;
    int number_of_used_frames;
    int number_of_keyframes;
    QPtrList<TLFrame> list_of_frames;
    TLFrame *last_used_frame;
    TLFrame *current_offset_frame;
    Ktoon *k_toon;

signals:
    /**
     * This signal is emitted when a frame has been inserted.
     */
    void frameInserted();
    /**
     * This signal is emitted when a frame has been removed.
     */
    void frameRemoved();
    /**
     * This signal is emitted when a keyframe has been removed.
     * @param pos_keyframe The position of the keyframe within this frame sequence
     */
    void keyframeRemoved( int pos_keyframe );
    /**
     * This signal is emitted when a motion tween has been created for a keyframe.
     * @param pos_keyframe The position of the keyframe within this frame sequence
     */
    void motionTweenCreated( int pos_keyframe );
    /**
     * This signal is emitted when a motion tween has been removed for a keyframe.
     * @param pos_keyframe The position of the keyframe within this frame sequence
     */
    void motionTweenRemoved( int pos_keyframe );

public slots:
    /**
     * @brief Inserts frames into this frame sequence
     *
     * This slot takes care of which frame is selected within this frame sequence. For instance, if no frame is
     * selected, it inserts a single frame at the end of this frame sequence; if is selected an used frame, it
     * inserts a frame after the selected frame, moving one position to the right all frames to its right side;
     * and if is selected an unused frame, it inserts frames at the end of the frame sequence until it reaches
     * the position of the selected frame.
     */
    void slotInsertFrame();
    /**
     * @brief Inserts a keyframe into this frame sequence
     *
     * For now, this slot is called only when a signal is activated in exposure sheet.
     */
    void slotInsertKeyframe();
    /**
     * @brief Removes a frame from this frame sequence
     *
     * As well as inserting a frame, this slot also takes care of which frame is selected within this frame
     * sequence. For example, if no frame is selected or is selected an unused frame, it removes the last frame;
     * and if is selected an used frame, this function removes that frame.
     */
    void slotRemoveFrame();
    /**
     * @brief Removes a keyframe from this frame sequence
     *
     * For now, this slot is called only when a signal is activated in exposure sheet.
     * @param block The keyframe position (relative to other keyframes) within this frame sequence
     */
    void slotRemoveKeyframeBlock( int block );
    /**
     * @brief Moves a keyframe with its associated block of frames to the left
     *
     * For now, this slot is called only when a signal is activated in exposure sheet.
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     * @sa slotMoveKeyframeRight()
     */
    void slotMoveKeyframeLeft( int key );
    /**
     * @brief Moves a keyframe with its associated block of frames to the right
     *
     * For now, this slot is called only when a signal is activated in exposure sheet.
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     * @sa slotMoveKeyframeLeft()
     */
    void slotMoveKeyframeRight( int key );
    /**
     * @brief Creates a motion tween for the selected keyframe
     *
     * It turns all the frames belonging to the selected keyframe into motion or unknown motion frames.
     * See TLFrame for an explanation of what is a motion or unknown motion frame.
     * @sa slotRemoveMotionTween()
     */
    void slotCreateMotionTween();
    /**
     * @brief Removes the motion tween of the selected keyframe if it has
     *
     * It sets to false the motion or unknown motion property of the frames belonging to the selected keyframe.
     * See TLFrame for an explanation of what is a motion or unknown motion frame.
     * @sa slotCreateMotionTween()
     */
    void slotRemoveMotionTween();

protected:
    /**
     * @brief Sets all the frames belonging a keyframe to be unknown motion frames
     *
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     * @sa createMotionFrames()
     */
    void createUnknownMotionFrames( TLFrame *key );
    /**
     * @brief Sets all the frames belonging a keyframe to be motion frames
     *
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     * @sa createUnknownMotionFrames()
     */
    void createMotionFrames( TLFrame *key );
    /**
     * @brief Sets all the frames belonging a keyframe to be normal frames (without motion or unknown motion)
     *
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     */
    void cleanMotionFrames( TLFrame *key );
    /**
     * @brief Sets to <b>false</b> the "has_drawing" property to all frames belonging a keyframe
     *
     * @param key The keyframe position (relative to other keyframes) within this frame sequence
     */
    void clearDrawingState( TLFrame *key );
    /**
     * @brief Updates the offsets of all keyframes
     *
     * Called after a frame or keyframe insertion or deletion. It assumes that each keyframe length is correct.
     */
    void updateKeyframeOffsets();

};

#endif
