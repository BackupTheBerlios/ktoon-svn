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

#include "tlframesequence.h"
#include "tlframesequencelayout.h"
#include "tlruler.h"
#include "tlframesequencemanager.h"
#include "ktoon.h"

//--------------- CONSTRUCTOR --------------------

TLFrameSequence::TLFrameSequence( int in_position, QWidget *parent, QWidget *grandparent )
    : QWidget( parent )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );
    
    //Initializations
    resize( 3000, 24 );
    position_ = in_position;
    parent_widget = parent;
    grandparent_widget = grandparent;
    number_of_used_frames = 1;
    number_of_keyframes = 1;
    k_toon = ( Ktoon * )( grandparent_widget -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() );

    //Initialize the list of frames
    TLFrame *next_frame;
    for ( int i = 1; i <= TL_MAX_NUMBER_OF_FRAMES; i++ )
    {
	next_frame = new TLFrame( this );
	next_frame -> move( list_of_frames.count() * next_frame -> width(), 0 );
	if ( i % 5 == 0 )
	    next_frame -> setSpecial( true );
	if ( i == 1 )
	{
	    next_frame -> setKey( true );
	    next_frame -> setLast( true );
	    last_used_frame = next_frame;
	}
	connect( next_frame, SIGNAL( selected() ), grandparent_widget, SLOT( slotSelectFrame() ) );
	list_of_frames.append( next_frame );
    }
    TLFrameSequenceManager *fsm = ( TLFrameSequenceManager * )grandparent -> parentWidget();
    current_offset_frame = next_frame;
    changeOffsetFrame( fsm -> getRuler() -> getOffset() );
}

//--------------- DESTRUCTOR --------------------

TLFrameSequence::~TLFrameSequence()
{

}

//-------------- PUBLIC MEMBERS ----------------

int TLFrameSequence::position()
{
    return position_;
}

int TLFrameSequence::numberOfUsedFrames()
{
    return number_of_used_frames;
}

int TLFrameSequence::numberOfKeyframes()
{
    return number_of_keyframes;
}

void TLFrameSequence::setPosition( int in_position )
{
    position_ = in_position;
}

void TLFrameSequence::changeOffsetFrame( int new_offset )
{
    if ( new_offset < 1 )
        return;
    current_offset_frame -> setOffset( false );
    current_offset_frame -> setDragOffset( false );
    current_offset_frame = list_of_frames.at( new_offset - 1 );
    current_offset_frame -> setDragOffset( false );
    current_offset_frame -> setOffset( true );
}

void TLFrameSequence::changeDragOffsetFrame( int new_offset )
{
    if ( new_offset < 1 )
        return;
    current_offset_frame -> setOffset( false );
    current_offset_frame -> setDragOffset( false );
    current_offset_frame = list_of_frames.at( new_offset - 1 );
    current_offset_frame -> setOffset( false );
    current_offset_frame -> setDragOffset( true );
}

void TLFrameSequence::loadFrames( QPtrList<KeyFrame> keyframes )
{
    list_of_frames.at( 0 ) -> setUsed( false );
    number_of_used_frames = 0;
    number_of_keyframes = 0;

    KeyFrame *k_it;
    TLFrame *f_it;
    for ( k_it = keyframes.first(), f_it = list_of_frames.first(); k_it; k_it = keyframes.next(), f_it = list_of_frames.next() )
    {
	QPtrList<GLGraphicComponent> gc = k_it -> getDrawing() -> graphicComponents();
        number_of_keyframes++;
	f_it -> setKey( true );
	if ( k_it == keyframes.getLast() )
	    f_it -> setUnknownMotion( k_it -> motionKeyFrame() );
	else
	    f_it -> setMotion( k_it -> motionKeyFrame() );
	if ( !gc.isEmpty() )
	    f_it -> setHasDrawing( true );
	int i;
	for ( i = 1; i <= k_it -> lengthKeyFrame() - 1; i++, f_it = list_of_frames.next() )
	{
	    f_it -> setUsed( true );
	    if ( k_it == keyframes.getLast() )
	        f_it -> setUnknownMotion( k_it -> motionKeyFrame() );
	    else
	        f_it -> setMotion( k_it -> motionKeyFrame() );
	    number_of_used_frames++;
	    if ( !gc.isEmpty() )
	        f_it -> setHasDrawing( true );
	}
	f_it -> setLast( true );
	if ( k_it == keyframes.getLast() )
	    f_it -> setUnknownMotion( k_it -> motionKeyFrame() );
	else
	    f_it -> setMotion( k_it -> motionKeyFrame() );
	number_of_used_frames++;
	if ( !gc.isEmpty() )
	    f_it -> setHasDrawing( true );
	last_used_frame = f_it;
    }
}

TLFrame *TLFrameSequence::findPrevKeyframe( TLFrame *frame )
{
    Q_CHECK_PTR( frame );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( frame ) - 1 ); frame_iterator; frame_iterator = list_of_frames.prev() )
    {
	if ( frame_iterator -> isKey() )
	    return frame_iterator;
    }
    return NULL;
}

TLFrame *TLFrameSequence::findNextKeyframe( TLFrame *frame )
{
    Q_CHECK_PTR( frame );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( frame ) + 1 ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    return frame_iterator;
    }
    return NULL;
}

int TLFrameSequence::keyframePosition( TLFrame *frame )
{
    Q_CHECK_PTR( frame );
    TLFrame *frame_iterator;
    int res = 0;
    for ( frame_iterator = list_of_frames.first(); frame_iterator != frame; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    res++;
    }
    if ( frame_iterator ->isKey() )
        res++;
	
    return res;
}

int TLFrameSequence::framePosition( TLFrame *frame )
{
    Q_CHECK_PTR( frame );
    return list_of_frames.find( frame ) + 1;
}

TLFrame *TLFrameSequence::offsetFrame()
{
    return current_offset_frame;
}

void TLFrameSequence::setKeyframeHasDrawing( int pos, bool state )
{
    TLFrame *key = NULL, *frame_iterator;
    int i = 0;

    //Find the keyframe
    for ( frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
        if ( frame_iterator -> isKey() )
	    i++;
	if ( i == pos )
	{
	    key = frame_iterator;
	    break;
	}
    }

    for ( frame_iterator = list_of_frames.at( list_of_frames.find( key ) ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	frame_iterator -> setHasDrawing( state );
	if ( frame_iterator -> isLast() )
	    break;
    }
}

//-------------- SLOTS --------------------

void TLFrameSequence::slotInsertFrame()
{
    TLFrameSequenceLayout *fsl_tmp = ( TLFrameSequenceLayout * )grandparent_widget;
    TLFrame *selected_frame = fsl_tmp -> currentFrame();

    //Prevent an overload exception
    if ( number_of_used_frames == TL_MAX_NUMBER_OF_FRAMES )
        return;

    //Case 1: When no frame is selected, insert a frame after the last frame
    if ( selected_frame == NULL )
    {
	last_used_frame -> setLast( false );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) + 1 );
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> isUnknownMotion() )
	    last_used_frame -> setUnknownMotion( true );
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> hasDrawing() )
	    last_used_frame -> setHasDrawing( true );
	last_used_frame -> setLast( true );
	number_of_used_frames++;

    	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	KeyFrame *ckf = kf.getLast();
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );

	emit frameInserted();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }

    //Case 2: When an used frame is selected, insert a frame after it
    if ( selected_frame == last_used_frame )
    {
	last_used_frame -> setLast( false );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) + 1 );
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> isUnknownMotion() )
	    last_used_frame -> setUnknownMotion( true );
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> hasDrawing() )
	    last_used_frame -> setHasDrawing( true );
	last_used_frame -> setLast( true );
	number_of_used_frames++;

    	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	KeyFrame *ckf = kf.getLast();
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );
	updateKeyframeOffsets();

	emit frameInserted();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }
    else if ( selected_frame -> isLast() && selected_frame != last_used_frame )
    {
        TLFrame *frame_iterator1, *frame_iterator2, *new_frame;
	int i, j;
	//Move all frames after the selected frame to the right
	for ( i = list_of_frames.find( last_used_frame ), j = list_of_frames.find( last_used_frame ) + 1;
	      i >= list_of_frames.find( selected_frame ); i--, j-- )
	{
	    if ( j == list_of_frames.find( last_used_frame ) + 1 )
	        last_used_frame = list_of_frames.at( j );
	    frame_iterator1 = list_of_frames.at( i );
	    frame_iterator2 = list_of_frames.at( j );
	    frame_iterator2 -> setUseProperties( frame_iterator1 );
	}
	selected_frame -> setLast( false );
	new_frame = list_of_frames.at( list_of_frames.find( selected_frame ) + 1 );
	if ( selected_frame -> isMotion() )
	    new_frame -> setMotion( true );
	if ( selected_frame -> hasDrawing() )
	    new_frame -> setHasDrawing( true );
	new_frame -> setLast( true );
	new_frame -> setKey( false );
	number_of_used_frames++;

    	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
        //Get the position of the keyframe into the variable "i"
        i = 0;
	if ( selected_frame -> isKey() )
	    i = 1;
	TLFrame *frame_iterator;
        for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
        {
	    if ( frame_iterator -> isKey() )
	        i++;
        }
	KeyFrame *ckf = kf.at( i - 1 );
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );
	updateKeyframeOffsets();

	emit frameInserted();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }
    else if ( !( selected_frame -> isLast() ) && selected_frame -> isUsed() )
    {
        TLFrame *frame_iterator1, *frame_iterator2, *new_frame;
	int i, j;
	//Move all frames after the selected frame to the right
	for ( i = list_of_frames.find( last_used_frame ), j = list_of_frames.find( last_used_frame ) + 1;
	      i >= list_of_frames.find( selected_frame ) + 1; i--, j-- )
	{
	    if ( j == list_of_frames.find( last_used_frame ) + 1 )
	        last_used_frame = list_of_frames.at( j );
	    frame_iterator1 = list_of_frames.at( i );
	    frame_iterator2 = list_of_frames.at( j );
	    frame_iterator2 -> setUseProperties( frame_iterator1 );
	}
	new_frame = list_of_frames.at( list_of_frames.find( selected_frame ) + 1 );
	new_frame -> setLast( false );
	new_frame -> setKey( false );
	new_frame -> setUsed( true );
	number_of_used_frames++;

    	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
        //Get the position of the keyframe into the variable "i"
        i = 0;
	if ( selected_frame -> isKey() )
	    i = 1;
	TLFrame *frame_iterator;
        for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
        {
	    if ( frame_iterator -> isKey() )
	        i++;
        }
	KeyFrame *ckf = kf.at( i - 1 );
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );
	updateKeyframeOffsets();

	emit frameInserted();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }

    //Case 3: When an unused frame is selected, insert frames from the last to the selected frame
    if ( !( selected_frame -> isUsed() ) )
    {
        TLFrame *frame_iterator, *bridge_frame;
	bridge_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) + 1 );

	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	KeyFrame *ckf = kf.getLast();

	for( frame_iterator = bridge_frame; frame_iterator != selected_frame; frame_iterator = list_of_frames.next() )
	{
	    frame_iterator -> setUsed( true );
	    if ( last_used_frame -> isUnknownMotion() )
	        frame_iterator -> setUnknownMotion( true );
	    if ( last_used_frame -> hasDrawing() )
	        frame_iterator -> setHasDrawing( true );
	    number_of_used_frames++;

	    ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );
	}
	last_used_frame -> setLast( false );
	last_used_frame = selected_frame;
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> isUnknownMotion() )
	    last_used_frame -> setUnknownMotion( true );
	if ( ( list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 ) ) -> hasDrawing() )
	    last_used_frame -> setHasDrawing( true );
	last_used_frame -> setLast( true );
	number_of_used_frames++;

	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() + 1 );
	updateKeyframeOffsets();

	emit frameInserted();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }
}

void TLFrameSequence::slotInsertKeyframe()
{
    TLFrameSequenceLayout *fsl_tmp = ( TLFrameSequenceLayout * )grandparent_widget;
    TLFrame *selected_frame = fsl_tmp -> currentFrame();

    //Case 1: When no frame is selected, insert a keyframe after the last frame
    if ( selected_frame == NULL )
    {
        //Prevent an overload exception
        if ( number_of_used_frames == TL_MAX_NUMBER_OF_FRAMES )
            return;
	if ( last_used_frame -> isUnknownMotion() )
	    createMotionFrames( findPrevKeyframe( last_used_frame ) );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) + 1 );
	last_used_frame -> setKey( true );
	last_used_frame -> setLast( true );
	number_of_used_frames++;
	number_of_keyframes++;

	updateKeyframeOffsets();
	emit frameInserted();
	return;
    }

    //Case 2: When the selection is already a keyframe, do nothing
    if ( selected_frame -> isKey() )
        return;

    //Case 3: When the selection is an used frame
    if ( selected_frame -> isUsed() )
    {
        TLFrame *prev_frame = list_of_frames.at( list_of_frames.find( selected_frame ) - 1 );
	selected_frame -> setKey( true );
	prev_frame -> setLast( true );
	number_of_keyframes++;
	emit frameInserted();
	return;
    }

    //Case 4: When the selection is an unused frame
    if ( !( selected_frame -> isUsed() ) )
    {
        TLFrame *frame_iterator, *bridge_frame, *prev_frame;
	bridge_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) + 1 );
	for ( frame_iterator = bridge_frame; frame_iterator != selected_frame; frame_iterator = list_of_frames.next() )
	{
	    frame_iterator -> setUsed( true );
	    number_of_used_frames++;
	}
	last_used_frame -> setLast( false );
	last_used_frame = selected_frame;
	prev_frame = list_of_frames.at( list_of_frames.find( selected_frame ) - 1 );
	prev_frame -> setLast( true );
	last_used_frame -> setKey( true );
	last_used_frame -> setLast( true );
	number_of_used_frames++;
	number_of_keyframes++;
	emit frameInserted();
        return;
    }
}

void TLFrameSequence::slotRemoveFrame()
{
    TLFrameSequenceLayout *fsl_tmp = ( TLFrameSequenceLayout * )grandparent_widget;
    TLFrame *selected_frame = fsl_tmp -> currentFrame();

    //If there is only one frame, do nothing
    if ( number_of_used_frames == 1 )
        return;

    //Case 1: When no frame is selected, or if the selected frame is the last used frame, remove the last frame
    if ( selected_frame == NULL || selected_frame == last_used_frame )
    {
        if ( last_used_frame -> isKey() )
	{
	    number_of_keyframes--;
	    emit keyframeRemoved( number_of_keyframes + 1 );
	    TLFrame *prev_keyframe = findPrevKeyframe( last_used_frame );
	    if ( prev_keyframe -> isMotion() )
	        createUnknownMotionFrames( prev_keyframe );
	}
	last_used_frame -> setUsed( false );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 );
	last_used_frame -> setLast( true );
	number_of_used_frames--;

    	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	KeyFrame *ckf = kf.getLast();
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() - 1 );

	emit frameRemoved();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }

    //Case 2: If the selected frame is an used frame but it's neither last nor key
    if ( selected_frame -> isUsed() && !( selected_frame -> isKey() ) && !( selected_frame -> isLast() ) )
    {
        TLFrame *frame_iterator1, *frame_iterator2;
	int i, j;
	//Move all frames after the selected frame to the left
	for ( i = list_of_frames.find( selected_frame ), j = list_of_frames.find( selected_frame ) + 1;
	      j < list_of_frames.find( last_used_frame ) + 1; i++, j++ )
	{
	    frame_iterator1 = list_of_frames.at( i );
	    frame_iterator2 = list_of_frames.at( j );
	    frame_iterator1 -> setUseProperties( frame_iterator2 );
	}
	last_used_frame -> setUsed( false );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 );
	number_of_used_frames--;

	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
        //Get the position of the keyframe into the variable "i"
        i = 0;
	TLFrame *frame_iterator;
        for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
        {
	    if ( frame_iterator -> isKey() )
	        i++;
        }
	KeyFrame *ckf = kf.at( i - 1 );
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() - 1 );
	updateKeyframeOffsets();

	emit frameRemoved();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }

    //Case 3: When the selected frame is a last frame
    if ( selected_frame -> isLast() )
    {
        if ( selected_frame -> isKey() )
	{
	    TLFrame *frame_it;
	    int i = 0;
            for ( frame_it = list_of_frames.first(); frame_it; frame_it = list_of_frames.next() )
    	    {
		if ( frame_it -> isKey() )
	    	    i++;
		if ( frame_it == selected_frame )
		    break;
	    }
	    number_of_keyframes--;
	    emit keyframeRemoved( i );
	}
        TLFrame *frame_iterator1, *frame_iterator2, *prev_frame;
	int i, j;
	//Move all frames after the selected frame to the left
	for ( i = list_of_frames.find( selected_frame ), j = list_of_frames.find( selected_frame ) + 1;
	      j < list_of_frames.find( last_used_frame ) + 1; i++, j++ )
	{
	    frame_iterator1 = list_of_frames.at( i );
	    frame_iterator2 = list_of_frames.at( j );
	    frame_iterator1 -> setUseProperties( frame_iterator2 );
	}
	last_used_frame -> setUsed( false );
	prev_frame = list_of_frames.at( list_of_frames.find( selected_frame ) - 1 );
	if ( prev_frame != 0 )
	    prev_frame -> setLast( true );
	else
	    selected_frame -> setKey( true );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 );
	number_of_used_frames--;

	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
        //Get the position of the keyframe into the variable "i"
        i = 0;
	TLFrame *frame_iterator;
        for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
        {
	    if ( frame_iterator -> isKey() )
	        i++;
        }
	if ( i - 1 == -1 )
	    i = 1;
	KeyFrame *ckf = kf.at( i - 1 );
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() - 1 );
	updateKeyframeOffsets();

	emit frameRemoved();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }

    //Case 4: When the selected frame is a key frame
    if ( selected_frame -> isKey() && !( selected_frame -> isLast() ) )
    {
        TLFrame *frame_iterator1, *frame_iterator2;
	int i, j;
	//Move all frames two steps after the selected frame to the left
	if ( ( list_of_frames.at( list_of_frames.find( selected_frame ) + 1 ) ) -> isLast() )
	    selected_frame -> setLast( true );
	for ( i = list_of_frames.find( selected_frame ) + 1, j = list_of_frames.find( selected_frame ) + 2;
	      j < list_of_frames.find( last_used_frame ) + 1; i++, j++ )
	{
	    frame_iterator1 = list_of_frames.at( i );
	    frame_iterator2 = list_of_frames.at( j );
	    frame_iterator1 -> setUseProperties( frame_iterator2 );
	}
	last_used_frame -> setUsed( false );
	last_used_frame = list_of_frames.at( list_of_frames.find( last_used_frame ) - 1 );
	number_of_used_frames--;

	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
        //Get the position of the keyframe into the variable "i"
        i = 1;
	TLFrame *frame_iterator;
        for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
        {
	    if ( frame_iterator -> isKey() )
	        i++;
        }
	KeyFrame *ckf = kf.at( i - 1 );
	ckf -> setLengthKeyFrame( ckf -> lengthKeyFrame() - 1 );
	updateKeyframeOffsets();

	emit frameRemoved();
	k_toon -> exposureSheet() -> updateIndicators( k_toon -> exposureSheet() -> currentLayerObj() );
	return;
    }
}

void TLFrameSequence::slotRemoveKeyframeBlock( int block )
{
    int i = 0, j = 0;
    TLFrame *frame_iterator, *kf_to_remove, *prev_kf;

    kf_to_remove = NULL;

    if ( block > number_of_keyframes || number_of_keyframes == 1 || block < 1 )
        return;

    if ( block == 1 )
    {
        for ( frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
	{
	    if ( frame_iterator -> isKey() )
	        j++;
	    if ( j == 2 )
	    {
	        kf_to_remove = frame_iterator;
	        break;
	    }
	}
	prev_kf = list_of_frames.at( list_of_frames.find( kf_to_remove ) - 1 );
	prev_kf -> setLast( false );
	kf_to_remove -> setKey( false );
	if ( kf_to_remove -> isUnknownMotion() )
	    createUnknownMotionFrames( list_of_frames.getFirst() );
	else if ( kf_to_remove -> isMotion() )
	    createMotionFrames( list_of_frames.getFirst() );
        if ( block == number_of_keyframes && ( list_of_frames.getFirst() ) -> isMotion() )
	    createUnknownMotionFrames( list_of_frames.getFirst() );
	if ( !( kf_to_remove -> hasDrawing() ) )
	    clearDrawingState( list_of_frames.getFirst() );
	number_of_keyframes--;
	return;
    }

    //Find the keyframe associated to the block
    for ( frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
	if ( i == block )
	{
	    kf_to_remove = frame_iterator;
	    break;
	}
    }
    TLFrame *tmp = findPrevKeyframe( kf_to_remove );
    prev_kf = list_of_frames.at( list_of_frames.find( kf_to_remove ) - 1 );
    if ( kf_to_remove -> isUnknownMotion() || kf_to_remove -> isMotion() )
	cleanMotionFrames( kf_to_remove );
    if ( kf_to_remove -> hasDrawing() )
        clearDrawingState( kf_to_remove );
    prev_kf -> setLast( false );
    kf_to_remove -> setKey( false );
    if ( tmp -> isUnknownMotion() )
	createUnknownMotionFrames( tmp );
    else if ( tmp -> isMotion() )
	createMotionFrames( tmp );
    if ( block == number_of_keyframes && tmp -> isMotion() )
	createUnknownMotionFrames( tmp );
    number_of_keyframes--;
}

void TLFrameSequence::slotMoveKeyframeLeft( int key )
{
    QPtrList<TLFrame> prev_block, current_block;
    TLFrame *frame_iterator, *frame_iterator2, *initial_prev;

    initial_prev = NULL;

    TLFrame *new_f;
    int i;

    //Get the block of frames previous to the current
    for ( i = 0, frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
	if ( i == key - 1 )
	{
	    frame_iterator2 = frame_iterator;
	    initial_prev = frame_iterator;
	    for ( ; frame_iterator2; frame_iterator2 = list_of_frames.next() )
	    {
	        new_f = new TLFrame( frame_iterator2 );
		prev_block.append( new_f );
		if ( frame_iterator2 -> isLast() )
		    break;
	    }
	    break;
	}
    }

    //Get the current block of frames
    for ( i = 0, frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
	if ( i == key )
	{
	    frame_iterator2 = frame_iterator;
	    for ( ; frame_iterator2; frame_iterator2 = list_of_frames.next() )
	    {
	        new_f = new TLFrame( frame_iterator2 );
		current_block.append( new_f );
		if ( frame_iterator2 -> isLast() )
		    break;
	    }
	    break;
	}
    }

    //Swap
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( initial_prev ) ), frame_iterator2 = current_block.first();
          frame_iterator2; frame_iterator = list_of_frames.next(), frame_iterator2 = current_block.next() )
	frame_iterator -> setUseProperties( frame_iterator2 );
    for ( frame_iterator2 = prev_block.first(); frame_iterator2;
          frame_iterator = list_of_frames.next(), frame_iterator2 = prev_block.next() )
	frame_iterator -> setUseProperties( frame_iterator2 );

    if ( key == number_of_keyframes && initial_prev -> isUnknownMotion() )
        createMotionFrames( initial_prev );

    updateKeyframeOffsets();
}

void TLFrameSequence::slotMoveKeyframeRight( int key )
{
    QPtrList<TLFrame> next_block, current_block;
    TLFrame *frame_iterator, *frame_iterator2, *initial_cur;

    initial_cur = NULL;

    TLFrame *new_f;
    int i;

    //Get the current block of frames
    for ( i = 0, frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
	if ( i == key )
	{
	    frame_iterator2 = frame_iterator;
	    initial_cur = frame_iterator;
	    for ( ; frame_iterator2; frame_iterator2 = list_of_frames.next() )
	    {
	        new_f = new TLFrame( frame_iterator2 );
		current_block.append( new_f );
		if ( frame_iterator2 -> isLast() )
		    break;
	    }
	    break;
	}
    }

    //Get the block of frames next to the current
    for ( i = 0, frame_iterator = list_of_frames.first(); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
	if ( i == key + 1 )
	{
	    frame_iterator2 = frame_iterator;
	    for ( ; frame_iterator2; frame_iterator2 = list_of_frames.next() )
	    {
	        new_f = new TLFrame( frame_iterator2 );
		next_block.append( new_f );
		if ( frame_iterator2 -> isLast() )
		    break;
	    }
	    break;
	}
    }

    //Swap
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( initial_cur ) ), frame_iterator2 = next_block.first();
          frame_iterator2; frame_iterator = list_of_frames.next(), frame_iterator2 = next_block.next() )
	frame_iterator -> setUseProperties( frame_iterator2 );
    for ( frame_iterator2 = current_block.first(); frame_iterator2;
          frame_iterator2 = current_block.next(), frame_iterator = list_of_frames.next() )
	frame_iterator -> setUseProperties( frame_iterator2 );

    if ( key == number_of_keyframes - 1 && ( current_block.getFirst() ) -> isMotion() )
        createUnknownMotionFrames( findPrevKeyframe( last_used_frame ) );

    updateKeyframeOffsets();
}

void TLFrameSequence::slotCreateMotionTween()
{
    TLFrameSequenceLayout *fsl_tmp = ( TLFrameSequenceLayout * )grandparent_widget;
    TLFrame *selected_frame = fsl_tmp -> currentFrame();
    TLFrame *frame_iterator;

    //if the selected frame is an unused frame, do nothing
    if ( !( selected_frame -> isUsed() ) )
        return;

    //Get the position of the keyframe into the variable "i"
    int i = 0;
    for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
    }

    if ( selected_frame -> isKey() )
        i++;

    //If there is no keyframe after the selected frame, create an "unknown" motion tween
    if ( i == number_of_keyframes )
    {
        if ( selected_frame -> isKey() )
            createUnknownMotionFrames( selected_frame );
	else
            createUnknownMotionFrames( findPrevKeyframe( selected_frame ) );
    }
    //If there is a keyframe after the selected frame, create a motion tween
    else
    {
        if ( selected_frame -> isKey() )
            createMotionFrames( selected_frame );
	else
            createMotionFrames( findPrevKeyframe( selected_frame ) );
    }

    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *ckf = kf.at( i - 1 );
    ckf -> setMotionKeyFrame( true );

    emit motionTweenCreated( i );
}

void TLFrameSequence::slotRemoveMotionTween()
{
    TLFrameSequenceLayout *fsl_tmp = ( TLFrameSequenceLayout * )grandparent_widget;
    TLFrame *selected_frame = fsl_tmp -> currentFrame();
    TLFrame *frame_iterator;

    //if the selected frame is an unused frame, do nothing
    if ( !( selected_frame -> isUsed() ) )
        return;

    //Get the position of the keyframe into the variable "i"
    int i = 0;
    for ( frame_iterator = list_of_frames.first(); !( frame_iterator == selected_frame ); frame_iterator = list_of_frames.next() )
    {
	if ( frame_iterator -> isKey() )
	    i++;
    }

    if ( selected_frame -> isKey() )
    {
        i++;
        cleanMotionFrames( selected_frame );
    }
    else
        cleanMotionFrames( findPrevKeyframe( selected_frame ) );

    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *ckf = kf.at( i - 1 );
    ckf -> setMotionKeyFrame( false );

    emit motionTweenRemoved( i );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLFrameSequence::createUnknownMotionFrames( TLFrame *key )
{
    Q_CHECK_PTR( key );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( key ) ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
        frame_iterator -> setUnknownMotion( true );
	if ( frame_iterator -> isLast() )
	    break;
    }
}

void TLFrameSequence::createMotionFrames( TLFrame *key )
{
    Q_CHECK_PTR( key );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( key ) ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
	frame_iterator -> setMotion( true );
	if ( frame_iterator -> isLast() )
	    break;
    }
}

void TLFrameSequence::cleanMotionFrames( TLFrame *key )
{
    Q_CHECK_PTR( key );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( key ) ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
        frame_iterator -> setUnknownMotion( false );
	frame_iterator -> setMotion( false );
	if ( frame_iterator -> isLast() )
	    break;
    }
}

void TLFrameSequence::clearDrawingState( TLFrame *key )
{
    Q_CHECK_PTR( key );
    TLFrame *frame_iterator;
    for ( frame_iterator = list_of_frames.at( list_of_frames.find( key ) ); frame_iterator; frame_iterator = list_of_frames.next() )
    {
        frame_iterator -> setHasDrawing( false );
	if ( frame_iterator -> isLast() )
	    break;
    }
}

void TLFrameSequence::updateKeyframeOffsets()
{
    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *f_it;
    int accum = 1;
    for ( f_it = kf.first(); f_it; f_it = kf.next() )
    {
	f_it -> setOffsetKeyFrame( accum );
        accum = accum + f_it -> lengthKeyFrame();
    }
}
